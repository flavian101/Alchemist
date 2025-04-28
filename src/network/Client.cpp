#include "Client.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <thread>
#include <memory>
#include <chrono>
#include <mutex>

Client::Client()
    : ssl_context_(boost::asio::ssl::context::sslv23),
    socket_(io_context_, ssl_context_),
    isConnected_(false),
    isAuthenticated_(false),
    stopRendering_(false)  // <-- flag for rendering loop
{
    ssl_context_.set_default_verify_paths();
    ssl_context_.set_verify_mode(boost::asio::ssl::verify_peer);
}

Client::~Client() {
    stopChat();
}

bool Client::connectToServer(const std::string& host, const std::string& port) {
    try {
        std::cout << "Attempting to connect to server at " << host << ":" << port << std::endl;
        boost::asio::ip::tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::connect(socket_.lowest_layer(), endpoints);
        socket_.handshake(boost::asio::ssl::stream_base::client);

        isConnected_ = true;
        std::cout << "Connected to server." << std::endl;
        ioThread_ = std::make_unique<std::thread>(&Client::receiveMessages, this);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        isConnected_ = false;
        return false;
    }
}

bool Client::authenticate(const std::string& username, const std::string& password) {
    if (!isConnected_) return false;

    std::string auth = "AUTH " + username + " " + password + "\n";

    {
        std::lock_guard<std::mutex> lock(handlerMutex_);
        pendingResponseHandler_ = [this, username](const std::string& response) {
            if (response.find("Authentication successful") != std::string::npos) {
                std::cout << "Authentication successful.\n";
                isAuthenticated_ = true;
                authenticatedUsername_ = username;
            }
            else {
                std::cerr << "Authentication failed: " << response << std::endl;
            }
            };
    }

    try {
        boost::asio::write(socket_, boost::asio::buffer(auth));
        return true;  // Assume success; actual result comes through async message
    }
    catch (const std::exception& e) {
        std::cerr << "Authentication send failed: " << e.what() << std::endl;
        return false;
    }
}

bool Client::registerUser(const std::string& username, const std::string& password)
{
    try {
        std::string registration = "REGISTER " + username + " " + password + "\n";
        boost::asio::write(socket_, boost::asio::buffer(registration));

        boost::asio::streambuf buf;
        boost::system::error_code ec;
        boost::asio::read_until(socket_, buf, "\n", ec);

        if (ec) {
            std::cerr << "Error reading registration response: " << ec.message() << std::endl;
            return false;
        }

        std::istream is(&buf);
        std::string response;
        std::getline(is, response);

        std::cout << response << std::endl;
        return response.find("successful") != std::string::npos;
    }
    catch (const std::exception& e) {
        std::cerr << "Registration exception: " << e.what() << std::endl;
        return false;
    }
}

void Client::startChat() {
    if (!isAuthenticated_) {
        std::cerr << "You must authenticate before starting the chat." << std::endl;
        return;
    }

    stopRendering_ = false;
    // Instead of a busy loop, use a loop with a sleep or an event mechanism.
    while (!stopRendering_) {
        // Sleep briefly to prevent 100% CPU usage.
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Client::stopChat() {
    stopRendering_ = true;  // Signal to end the render loop
    if (isConnected_) {
        io_context_.stop();
        if (ioThread_ && ioThread_->joinable()) {
            ioThread_->join();
        }
        isConnected_ = false;
    }
}

void Client::sendMessageToServer(const std::string& message) {
    try {
        std::string formattedMessage = "CHAT " + message + "\n";
        boost::asio::write(socket_, boost::asio::buffer(formattedMessage));
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Boost system error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Generic error sending message: " << e.what() << std::endl;
    }

}

void Client::setMessageCallback(std::function<void(const std::string&)> callback)
{
    std::lock_guard<std::mutex> lock(messagesMutex_);
    messageCallback_ = std::move(callback);
}

void Client::processMessages()
{
    std::lock_guard<std::mutex> lock(messagesMutex_);
    while (!messageQueue_.empty()) {
        if (messageCallback_) {
            messageCallback_(messageQueue_.front());
        }
        messageQueue_.pop();
    }
}



void Client::loadProject(const std::string& projectId, std::function<void(const std::string&)> onSuccess)
{
    if(!isConnected_) return;

    // Set up the callback handler
    {
        std::lock_guard<std::mutex> lock(handlerMutex_);
        pendingResponseHandler_ = [this, projectId, onSuccess](const std::string& response) {
            if (response.find("PROJECT_DATA " + projectId) == 0) {
                size_t firstSpace = response.find(' ');
                size_t secondSpace = response.find(' ', firstSpace + 1);
                if (secondSpace != std::string::npos) {
                    std::string jsonData = response.substr(secondSpace + 1);
                    onSuccess(jsonData);
                }
                else {
                    std::cerr << "Malformed PROJECT_DATA response\n";
                }
            }
            };
    }

    // Send the load request
    std::string command = "LOAD_PROJECT " + projectId + "\n";
    boost::asio::write(socket_, boost::asio::buffer(command));
}




void Client::receiveMessages() {
    try {
        boost::asio::streambuf buf;
        while (isConnected_) {
            boost::system::error_code ec;
            boost::asio::read_until(socket_, buf, "\n", ec);

            if (ec) {
                std::cerr << "Receive error: " << ec.message() << "\n";
                isConnected_ = false;
                break;
            }

            std::istream is(&buf);
            std::string line;
            std::getline(is, line);

            // Check for pending handler
            {
                std::lock_guard<std::mutex> lock(handlerMutex_);
                if (pendingResponseHandler_) {
                    auto handler = std::move(pendingResponseHandler_);
                    pendingResponseHandler_ = nullptr;
                    handler(line);
                    continue; // Don’t queue this message
                }
            }

            handleIncomingMessage(line);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in receive thread: " << e.what() << "\n";
        isConnected_ = false;
    }
}

void Client::handleIncomingMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(messagesMutex_);
    messageQueue_.push(message);

    if (message.find("PROJECT_UPDATE ") == 0 && onProjectUpdateCallback_) {
        size_t idEnd = message.find(' ', 15); // after "PROJECT_UPDATE "
        if (idEnd != std::string::npos) {
            std::string projectId = message.substr(15, idEnd - 15);
            std::string jsonData = message.substr(idEnd + 1);
            onProjectUpdateCallback_(projectId, jsonData);
        }
        return;
    }

    // If callback is set, notify immediately
    if (messageCallback_) {
        messageCallback_(message);
    }
}
bool Client::saveProject(const std::string& projectId, const std::string& name, const std::string& jsonData) {
    try {
        std::string command = "SAVE_PROJECT " + projectId + " " + name + " " + jsonData + "\n";
        boost::asio::write(socket_, boost::asio::buffer(command));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to save project: " << e.what() << std::endl;
        return false;
    }
}
// Updates to Client.cpp for listProjects method
void Client::listProjects(std::function<void(const std::vector<std::pair<std::string, std::string>>&)> callback) {
    if (!isConnected_ || !socket_.lowest_layer().is_open()) {
        std::cerr << "[Error] Socket is not connected.\n";
        callback({}); // Return empty list
        return;
    }

    if (!isAuthenticated_) {
        std::cerr << "[Error] Client not authenticated.\n";
        callback({}); // Return empty list
        return;
    }

    std::string command = "LIST_PROJECTS " + authenticatedUsername_ + "\n";
    std::cout << "Sending command: " << command;

    {
        std::lock_guard<std::mutex> lock(handlerMutex_);
        pendingResponseHandler_ = [callback, this](const std::string& response) {
            std::vector<std::pair<std::string, std::string>> projects;
            std::cout << "Received project list response: '" << response << "'" << std::endl;

            if (response.find("PROJECT_LIST ") == 0) {
                std::string listPart = response.substr(13);

                // Trim whitespace
                listPart.erase(listPart.find_last_not_of(" \r\n") + 1);

                std::cout << "Project list data: '" << listPart << "'" << std::endl;

                // Check if list is empty
                if (listPart.empty()) {
                    std::cout << "Project list is empty" << std::endl;
                    callback(projects);
                    return;
                }

                size_t pos = 0;
                std::string token;
                const std::string delimiter = "|";

                while ((pos = listPart.find(delimiter)) != std::string::npos) {
                    token = listPart.substr(0, pos);

                    size_t sep = token.find(':');
                    if (sep != std::string::npos) {
                        std::string id = token.substr(0, sep);
                        std::string name = token.substr(sep + 1);
                        projects.emplace_back(id, name);
                        std::cout << "Found project: " << name << " (ID: " << id << ")" << std::endl;
                    }

                    listPart.erase(0, pos + delimiter.length());
                }
            }
            else {
                std::cerr << "Invalid response format for project list" << std::endl;
            }

            callback(projects);
            };
    }

    try {
        boost::asio::write(socket_, boost::asio::buffer(command));
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "[Boost Error] Failed to send listProjects: " << e.what() << "\n";
        callback({}); // Return empty list on error
    }
}
void Client::setProjectUpdateCallback(std::function<void(const std::string&, const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(messagesMutex_);
    onProjectUpdateCallback_ = std::move(callback);
}
bool Client::pushProject(const std::string& projectId, const std::string& name, const std::string& jsonData) {
    try {
        std::string command = "PUSH_PROJECT " + projectId + " " + name + " " + jsonData + "\n";
        boost::asio::write(socket_, boost::asio::buffer(command));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to push project update: " << e.what() << std::endl;
        return false;
    }
}

void Client::startSyncLoop() {
    while (isConnected_ && !stopRendering_) {
        // Every 5 seconds, synchronize with the server.
        std::this_thread::sleep_for(std::chrono::seconds(5));

        // Send request for the latest project data from the server
        listProjects([this](const std::vector<std::pair<std::string, std::string>>& projects) {
            for (const auto& project : projects) {
                std::cout << "Project: " << project.first << ", " << project.second << std::endl;
            }
            });

        // Optionally, push updates
        if (isAuthenticated_) {
            std::string projectData = "some_project_data"; // Get the data to sync
            pushProject("project_id", "project_name", projectData);
        }
    }
}
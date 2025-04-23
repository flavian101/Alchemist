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
        pendingResponseHandler_ = [this](const std::string& response) {
            if (response.find("Authentication successful") != std::string::npos) {
                std::cout << "Authentication successful.\n";
                isAuthenticated_ = true;
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

    // If callback is set, notify immediately
    if (messageCallback_) {
        messageCallback_(message);
    }
}

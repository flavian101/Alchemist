#include "Client.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <thread>
#include <memory> 

Client::Client()
    : ssl_context_(boost::asio::ssl::context::sslv23),
    socket_(io_context_, ssl_context_),
    chatWindow_(nullptr),
    isConnected_(false),
    isAuthenticated_(false)
{
    ssl_context_.set_default_verify_paths();
    ssl_context_.set_verify_mode(boost::asio::ssl::verify_peer);
  //  Client::work_guard_ = boost::asio::make_work_guard(io_context_);

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

        // Perform SSL handshake
        socket_.handshake(boost::asio::ssl::stream_base::client);

        isConnected_ = true;
        std::cout << "Connected to server." << std::endl;

        // Start receiving messages in a separate thread
        ioThread_ = std::make_unique<std::thread>(&Client::receiveMessages, this);
        return true; // Connection successful
    }
    catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        isConnected_ = false;
        return false; // Connection failed
    }
}

bool Client::authenticate(const std::string& username, const std::string& password) {
    try {
        std::string auth = "AUTH " + username + " " + password + "\n";
        boost::asio::write(socket_, boost::asio::buffer(auth));

        boost::asio::streambuf buf;
        boost::system::error_code ec;
        boost::asio::read_until(socket_, buf, "\n", ec);

        if (ec == boost::asio::error::eof || ec == boost::asio::ssl::error::stream_truncated) {
            std::cerr << "Server closed the connection after authentication failure.\n";
            return false;
        }
        else if (ec) {
            std::cerr << "Error reading authentication response: " << ec.message() << std::endl;
            return false;
        }

        std::istream is(&buf);
        std::string response;
        std::getline(is, response);

        if (response.find("Authentication successful") != std::string::npos) {
            std::cout << "Authentication successful.\n";
            isAuthenticated_ = true;
            chatWindow_ = std::make_shared<ChatWindow>(*this);
            return true;
        }
        else {
            std::cerr << "Authentication failed: " << response << std::endl;
            return false;  // UI will display error and let user retry
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Authentication exception: " << e.what() << std::endl;
        return false;
    }
}

void Client::startChat() {
    if (!isAuthenticated_) {
        std::cerr << "You must authenticate before starting the chat." << std::endl;
        return;
    }

    while (true) {
        render();
    }
}

void Client::stopChat() {
    if (isConnected_) {
        io_context_.stop();
        if (ioThread_ && ioThread_->joinable()) {
            ioThread_->join();
        }
        isConnected_ = false;

        // Reset the chat window before stopping
        chatWindow_.reset();
    }
}

void Client::sendMessageToServer(const std::string& message)
{
    try {
        boost::asio::write(socket_, boost::asio::buffer(message + "\n"));
    }
    catch (const std::exception& e) {
        std::cerr << "Error sending message: " << e.what() << std::endl;
    }
}

void Client::render() {
    // Ensure that we safely access the chat window in a thread-safe manner
    if (chatWindow_) {
        chatWindow_->render();
    }
}

void Client::receiveMessages() {
    try {
        while (isConnected_) {
            char buffer[1024] = { 0 }; // Initialize buffer to zeros

            boost::system::error_code ec;
            size_t bytes_transferred = socket_.read_some(boost::asio::buffer(buffer, sizeof(buffer)), ec);

            if (ec) {
                if (ec == boost::asio::error::eof) {
                    std::cerr << "Connection closed by server" << std::endl;
                }
                else {
                    std::cerr << "Error receiving messages: " << ec.message() << std::endl;
                }
                isConnected_ = false;
                break;
            }

            if (bytes_transferred > 0) {
                std::string message(buffer, bytes_transferred);
                handleIncomingMessage(message);
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in receive thread: " << e.what() << std::endl;
        isConnected_ = false;
    }
}

void Client::handleIncomingMessage(const std::string& message) {
    if (chatWindow_) {
        chatWindow_->addMessage("Server: " + message);
    }
}

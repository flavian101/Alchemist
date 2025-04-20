#include "NetworkServer.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <thread>
#include <algorithm>
#include <mutex>
#include <ctime>

NetworkServer::NetworkServer(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context, short port, DatabaseManager& dbManager)
    : io_context_(io_context),
    ssl_context_(ssl_context),
    acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    dbManager_(dbManager)
{
    startAccept();
}

void NetworkServer::startAccept() {
    auto socket = std::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>(io_context_, ssl_context_);
    acceptor_.async_accept(socket->lowest_layer(), [this, socket](const boost::system::error_code& error) {
        handleAccept(error, socket);
        startAccept(); // Continue accepting new clients
        });
}

void NetworkServer::handleAccept(const boost::system::error_code& error, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket) {
    if (!error) {
        std::cout << "New client connected!" << std::endl;
        socket->async_handshake(boost::asio::ssl::stream_base::server, [this, socket](const boost::system::error_code& error) {
            if (!error) {
                handleClient(socket);
            }
            else {
                std::cerr << "Handshake error: " << error.message() << std::endl;
            }
            });

        {
            std::lock_guard<std::mutex> lock(clientMutex_);
            clients_.push_back(socket);
        }
    }
    else {
        std::cerr << "Accept error: " << error.message() << std::endl;
    }
}

void NetworkServer::handleClient(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket) {
    auto buffer = std::make_shared<std::array<char, 1024>>();
    socket->async_read_some(boost::asio::buffer(*buffer),
        [this, socket, buffer](const boost::system::error_code& ec, size_t len) {
            if (!ec) {
                std::string msg(buffer->data(), len);
                std::cout << "Received: " << msg << std::endl;

                if (msg.find("AUTH") == 0) {
                    auto parts = msg.substr(5);
                    auto pos = parts.find(' ');
                    if (pos != std::string::npos) {
                        auto username = parts.substr(0, pos);
                        auto password = parts.substr(pos + 1);
                        // Remove potential whitespace and newline
                        password.erase(password.find_last_not_of(" \r\n") + 1);
                        authenticateClient(username, password, socket);
                    }
                }
                else {
                    processChatMessage(msg, socket);
                }
                // Schedule another asynchronous read.
                handleClient(socket);
            }
            else {
                std::cerr << "Read error: " << ec.message() << std::endl;
                std::lock_guard<std::mutex> lock(clientMutex_);
                authenticatedClients_.erase(socket);
                clients_.erase(std::remove(clients_.begin(), clients_.end(), socket), clients_.end());
            }
        });
}

void NetworkServer::authenticateClient(const std::string& username,
    const std::string& password,
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket)
{
    if (dbManager_.authenticateUser(username, password)) {
        {
            std::lock_guard<std::mutex> lock(clientMutex_);
            authenticatedClients_[socket] = true;
        }
        std::string successMessage = "Authentication successful.\n";
        boost::asio::async_write(*socket, boost::asio::buffer(successMessage),
            [](const boost::system::error_code& ec, std::size_t) {
                if (ec) {
                    std::cerr << "Error sending success message: " << ec.message() << std::endl;
                }
            });
    }
    else {
        std::string failureMessage = "Authentication failed.\n";
        boost::asio::async_write(*socket, boost::asio::buffer(failureMessage),
            [socket](const boost::system::error_code& ec, std::size_t) {
                if (!ec) {
                    socket->async_shutdown([socket](const boost::system::error_code& shutdown_ec) {
                        if (shutdown_ec) {
                            std::cerr << "Shutdown error: " << shutdown_ec.message() << std::endl;
                        }
                        else {
                            std::cout << "Client disconnected cleanly after auth failure.\n";
                        }
                        });
                }
                else {
                    std::cerr << "Error sending failure message: " << ec.message() << std::endl;
                }
            });
    }
}

void NetworkServer::processChatMessage(const std::string& message, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> sender) {
    // Verify authentication
    {
        std::lock_guard<std::mutex> lock(clientMutex_);
        if (authenticatedClients_.find(sender) == authenticatedClients_.end() || !authenticatedClients_[sender]) {
            std::string errorMsg = "You must be authenticated to send messages.\n";
            boost::asio::async_write(*sender, boost::asio::buffer(errorMsg),
                [](const boost::system::error_code& ec, std::size_t) {
                    if (ec) {
                        std::cerr << "Error sending error message: " << ec.message() << std::endl;
                    }
                });
            return;
        }
    }

    // Extract message content
    std::string msgContent;
    if (message.find("CHAT ") == 0) {
        msgContent = message.substr(5);
    }
    else {
        msgContent = message;
    }

    // Add timestamp
    std::time_t currentTime = std::time(nullptr);
    std::string timeStr = std::ctime(&currentTime);
    timeStr.pop_back(); // Remove trailing newline

    // Format final message with CHAT prefix
    std::string formattedMessage = "CHAT [" + timeStr + "] " + msgContent;

    // Broadcast to all clients
    BroadcastChatMessage(formattedMessage);

    // Callback if registered
    if (messageReceivedCallback_) {
        messageReceivedCallback_(msgContent);
    }
}

void NetworkServer::processChatMessage(const std::string& message) {
    std::string msgContent = (message.find("CHAT ") == 0) ? message.substr(5) : message;
    std::time_t currentTime = std::time(nullptr);
    std::string timeStr = std::ctime(&currentTime);
    timeStr.pop_back();
    std::string formattedMessage = "CHAT [" + timeStr + "] " + msgContent;
    BroadcastChatMessage(formattedMessage);

    if (messageReceivedCallback_) {
        messageReceivedCallback_(msgContent);
    }
}

void NetworkServer::BroadcastChatMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(clientMutex_);
    for (auto it = clients_.begin(); it != clients_.end();) {
        auto client = *it;
        if (authenticatedClients_.find(client) != authenticatedClients_.end() && authenticatedClients_[client]) {
            boost::asio::async_write(*client, boost::asio::buffer(message + "\n"),
                [this, client](const boost::system::error_code& error, std::size_t) {
                    if (error) {
                        std::cerr << "Error sending message to client: " << error.message() << std::endl;
                        std::lock_guard<std::mutex> lock(clientMutex_);
                        authenticatedClients_.erase(client);
                        clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
                    }
                });
            ++it;
        }
        else {
            it = clients_.erase(it);
        }
    }
}

void NetworkServer::setMessageReceivedCallback(const std::function<void(const std::string&)>& callback) {
    messageReceivedCallback_ = callback;
}

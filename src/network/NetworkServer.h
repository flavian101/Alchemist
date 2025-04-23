#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>
#include "DatabaseManager.h"

class NetworkServer : public std::enable_shared_from_this<NetworkServer> {
public:
    NetworkServer(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context, short port, DatabaseManager& dbManager);

    void startAccept();
    void handleAccept(const boost::system::error_code& error, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket);
    void handleClient(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket);
    void authenticateClient(const std::string& username, const std::string& password, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket);
	void RegisterUser(const std::string& username, const std::string& password, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket);
    void processChatMessage(const std::string& message, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> sender);
    void processChatMessage(const std::string& message);
    void BroadcastChatMessage(const std::string& message);

    void setMessageReceivedCallback(const std::function<void(const std::string&)>& callback);

    void handleIncomingMessage(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket, const std::string& message);

    void handleClientDisconnect(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket, const boost::system::error_code& ec);

private:
    boost::asio::io_context& io_context_;
    boost::asio::ssl::context& ssl_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    DatabaseManager& dbManager_;
    std::vector<std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>> clients_;
    std::unordered_map<std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>, bool> authenticatedClients_;
    std::function<void(const std::string&)> messageReceivedCallback_;
    std::mutex clientMutex_; // Mutex to protect access to shared client data
};

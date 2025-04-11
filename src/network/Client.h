#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <thread>
#include <memory> /
#include "window/ChatWindow.h" 

class Client {
public:
    Client();
    ~Client();

    bool connectToServer(const std::string& host, const std::string& port); 
    bool authenticate(const std::string& username, const std::string& password); 
    void startChat();
    void stopChat();
    void sendMessageToServer(const std::string& message);

private:
    void render();
    void receiveMessages();
    void handleIncomingMessage(const std::string& message);

    boost::asio::io_context io_context_;
    boost::asio::ssl::context ssl_context_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    //  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::unique_ptr<std::thread> ioThread_;
    std::shared_ptr<ChatWindow> chatWindow_; 
    bool isConnected_;
    bool isAuthenticated_;
};

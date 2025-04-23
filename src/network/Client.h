#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <thread>
#include <memory> 
#include "window/ChatWindow.h" 
#include <queue>

class Client {
public:
    Client();
    ~Client();

    bool connectToServer(const std::string& host, const std::string& port); 
    bool authenticate(const std::string& username, const std::string& password); 
    bool registerUser(const std::string& username, const std::string& password);
    void startChat();
    void stopChat();
    void sendMessageToServer(const std::string& message);

    void setMessageCallback(std::function<void(const std::string&)> callback);
    void processMessages();

    bool saveProject(const std::string& projectId, const std::string& name, const std::string& jsonData);
    void loadProject(const std::string& projectId, std::function<void(const std::string&)> onSuccess);
    void listProjects(std::function<void(const std::vector<std::pair<std::string, std::string>>&) > callback);
    std::function<void(const std::string& projectId, const std::string& jsonData)> onProjectUpdateCallback_;
    void setProjectUpdateCallback(std::function<void(const std::string&, const std::string&)> callback);

    bool pushProject(const std::string& projectId, const std::string& name, const std::string& jsonData);



private:
    std::string authenticatedUsername_ = "";
    void receiveMessages();
    void handleIncomingMessage(const std::string& message);

    boost::asio::io_context io_context_;
    boost::asio::ssl::context ssl_context_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    //  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::unique_ptr<std::thread> ioThread_;
    bool isConnected_;
    bool isAuthenticated_;
	bool stopRendering_;  // Flag to control rendering loop

    // Message queue with mutex for thread safety
    std::function<void(const std::string&)> pendingResponseHandler_;
    std::mutex handlerMutex_;
    std::mutex messagesMutex_;
    std::queue<std::string> messageQueue_;
    std::function<void(const std::string&)> messageCallback_;

};

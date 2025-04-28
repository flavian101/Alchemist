// Client.h
#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <functional>
#include <vector>

class Client {
public:
    Client();
    ~Client();

    // Connection & auth
    bool connectToServer(const std::string& host, const std::string& port);
    bool authenticate(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);

    // Chat
    void startChat();
    void stopChat();
    void sendMessageToServer(const std::string& message);

    // Project CRUD
    bool saveProject(const std::string& projectId, const std::string& name, const std::string& jsonData);
    void loadProject(const std::string& projectId, std::function<void(const std::string&)> onSuccess);
    void listProjects(std::function<void(const std::vector<std::pair<std::string, std::string>>&)> callback);
    bool pushProject(const std::string& projectId, const std::string& name, const std::string& jsonData);

    // Model upload/download
    bool uploadModel(const std::string& projectId, const std::string& modelPath,
        const char* data, size_t size);
    bool downloadModel(const std::string& projectId, const std::string& modelPath,
        std::function<void(const std::vector<char>&)> callback);

    // Callbacks
    void setMessageCallback(std::function<void(const std::string&)> cb);
    void setProjectUpdateCallback(std::function<void(const std::string&, const std::string&)> cb);
    void setModelUpdateCallback(std::function<void(const std::string&, const std::string&)> cb);

    bool isAuthenticated() const { return isAuthenticated_; }

    // Pump any queued chat messages
    void processMessages();

private:
    // IO
    void startReadLoop();
    void handleIncomingLine(const std::string& line);

    boost::asio::io_context            ioContext_;
    boost::asio::ssl::context          sslContext_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    std::unique_ptr<std::thread>       ioThread_;

    // State
    bool        isConnected_;
    bool        isAuthenticated_;
    std::string authenticatedUsername_;

    // Queues & mutexes
    std::mutex                                  handlerMutex_;
    std::function<void(const std::string&)>     pendingResponseHandler_;

    std::mutex                                  messagesMutex_;
    std::queue<std::string>                     messageQueue_;
    std::function<void(const std::string&)>     messageCallback_;

    std::function<void(const std::string&, const std::string&)> onProjectUpdateCallback_;
    std::function<void(const std::string&, const std::string&)> onModelUpdateCallback_;

    // For DOWNLOAD_MODEL callbacks
    std::mutex                                                                    downloadMutex_;
    std::unordered_map<std::string, std::function<void(const std::vector<char>&)>> modelDownloadCallbacks_;

    bool stopRendering_;
};

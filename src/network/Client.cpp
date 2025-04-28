// Client.cpp
#include "Client.h"
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include "Utility.h"   

Client::Client()
    : sslContext_(boost::asio::ssl::context::sslv23)
    , socket_(ioContext_, sslContext_)
    , isConnected_(false)
    , isAuthenticated_(false)
    , stopRendering_(false)
{
    sslContext_.set_default_verify_paths();
    sslContext_.set_verify_mode(boost::asio::ssl::verify_peer);
}

Client::~Client() {
    stopChat();
}

bool Client::connectToServer(const std::string& host, const std::string& port) {
    try {
        boost::asio::ip::tcp::resolver resolver(ioContext_);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::connect(socket_.lowest_layer(), endpoints);
        socket_.handshake(boost::asio::ssl::stream_base::client);

        isConnected_ = true;
        // start our async read loop
        startReadLoop();
        // and run the I/O context in its own thread
        ioThread_ = std::make_unique<std::thread>([this]() { ioContext_.run(); });
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << "\n";
        return false;
    }
}

bool Client::authenticate(const std::string& username, const std::string& password) {
    if (!isConnected_) return false;
    std::string msg = "AUTH " + username + " " + password + "\n";

    {
        std::lock_guard lock(handlerMutex_);
        pendingResponseHandler_ = [this, username](const std::string& resp) {
            if (resp.find("Authentication successful") != std::string::npos) {
                isAuthenticated_ = true;
                authenticatedUsername_ = username;
            }
            else {
                std::cerr << "Auth failed: " << resp << "\n";
            }
            };
    }

    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "Auth send failed: " << ec.message() << "\n"; });
    return true;
}

bool Client::registerUser(const std::string& username, const std::string& password) {
    if (!isConnected_) return false;
    std::string msg = "REGISTER " + username + " " + password + "\n";

    {
        std::lock_guard lock(handlerMutex_);
        pendingResponseHandler_ = [](const std::string& resp) {
            std::cout << resp << "\n";
            };
    }

    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "Register send failed: " << ec.message() << "\n"; });
    return true;
}

void Client::sendMessageToServer(const std::string& message) {
    if (!isConnected_) return;
    std::string msg = "CHAT " + message + "\n";
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "Chat send failed: " << ec.message() << "\n"; });
}

void Client::startChat() {
    stopRendering_ = false;
}

void Client::stopChat() {
    stopRendering_ = true;
    ioContext_.stop();
    if (ioThread_ && ioThread_->joinable()) ioThread_->join();
    isConnected_ = false;
}

// PROJECT I/O
bool Client::saveProject(const std::string& projectId, const std::string& name, const std::string& jsonData) {
    if (!isConnected_) return false;
    std::string msg = "SAVE_PROJECT " + projectId + " " + name + " " + jsonData + "\n";
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "SAVE_PROJECT failed: " << ec.message() << "\n"; });
    return true;
}

void Client::loadProject(const std::string& projectId, std::function<void(const std::string&)> onSuccess) {
    if (!isConnected_) return;
    {
        std::lock_guard lock(handlerMutex_);
        pendingResponseHandler_ = [projectId, onSuccess](const std::string& resp) {
            if (resp.rfind("PROJECT_DATA " + projectId, 0) == 0) {
                size_t p = resp.find(' ', resp.find(' ') + 1);
                onSuccess(resp.substr(p + 1));
            }
            };
    }
    std::string msg = "LOAD_PROJECT " + projectId + "\n";
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "LOAD_PROJECT failed: " << ec.message() << "\n"; });
}

void Client::listProjects(std::function<void(const std::vector<std::pair<std::string, std::string>>&)> callback) {
    if (!isAuthenticated_) { callback({}); return; }
    {
        std::lock_guard lock(handlerMutex_);
        pendingResponseHandler_ = [callback](const std::string& resp) {
            std::vector<std::pair<std::string, std::string>> out;
            if (resp.rfind("PROJECT_LIST ", 0) == 0) {
                std::string body = resp.substr(13);
                // trim newline
                while (!body.empty() && (body.back() == '\n' || body.back() == '\r')) body.pop_back();
                size_t pos = 0;
                while (true) {
                    auto bar = body.find('|', pos);
                    auto token = body.substr(pos, bar == std::string::npos ? std::string::npos : bar - pos);
                    auto c = token.find(':');
                    if (c != std::string::npos)
                        out.emplace_back(token.substr(0, c), token.substr(c + 1));
                    if (bar == std::string::npos) break;
                    pos = bar + 1;
                }
            }
            callback(out);
            };
    }
    std::string msg = "LIST_PROJECTS " + authenticatedUsername_ + "\n";
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "LIST_PROJECTS failed: " << ec.message() << "\n"; });
}

bool Client::pushProject(const std::string& projectId, const std::string& name, const std::string& jsonData) {
    if (!isConnected_) return false;
    std::string msg = "PUSH_PROJECT " + projectId + " " + name + " " + jsonData + "\n";
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "PUSH_PROJECT failed: " << ec.message() << "\n"; });
    return true;
}

// MODEL I/O
bool Client::uploadModel(const std::string& projectId, const std::string& modelPath,
    const char* data, size_t size)
{
    if (!isAuthenticated_) return false;
    auto encoded = base64_encode(reinterpret_cast<const unsigned char*>(data), size);
    std::string msg = "UPLOAD_MODEL " + projectId + " " + modelPath + " " + encoded + "\n";
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "UPLOAD_MODEL failed: " << ec.message() << "\n"; });
    return true;
}

bool Client::downloadModel(const std::string& projectId, const std::string& modelPath,
    std::function<void(const std::vector<char>&)> callback)
{
    if (!isAuthenticated_) return false;
    std::string key = projectId + ":" + modelPath;
    {
        std::lock_guard lock(downloadMutex_);
        modelDownloadCallbacks_[key] = std::move(callback);
    }
    std::string msg = "DOWNLOAD_MODEL " + projectId + " " + modelPath + "\n";
    boost::asio::async_write(socket_,
        boost::asio::buffer(msg),
        [](auto ec, auto) { if (ec) std::cerr << "DOWNLOAD_MODEL failed: " << ec.message() << "\n"; });
    return true;
}

// CALLBACK SETTERS
void Client::setMessageCallback(std::function<void(const std::string&)> cb) {
    std::lock_guard lock(messagesMutex_);
    messageCallback_ = std::move(cb);
}

void Client::setProjectUpdateCallback(std::function<void(const std::string&, const std::string&)> cb) {
    onProjectUpdateCallback_ = std::move(cb);
}

void Client::setModelUpdateCallback(std::function<void(const std::string&, const std::string&)> cb) {
    onModelUpdateCallback_ = std::move(cb);
}

// PROCESS QUEUED CHAT
void Client::processMessages() {
    std::lock_guard lock(messagesMutex_);
    while (!messageQueue_.empty()) {
        if (messageCallback_) messageCallback_(messageQueue_.front());
        messageQueue_.pop();
    }
}

// INTERNAL: start the async read‐until loop
void Client::startReadLoop() {
    auto buf = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(socket_, *buf, "\n",
        [this, buf](auto ec, std::size_t) {
            if (!ec) {
                std::istream is(buf.get());
                std::string line;
                std::getline(is, line);
                handleIncomingLine(line);
                // loop again:
                startReadLoop();
            }
        });
}

void Client::handleIncomingLine(const std::string& line) {
    // 1) pendingResponseHandler_
    {
        std::lock_guard lock(handlerMutex_);
        if (pendingResponseHandler_) {
            auto h = std::move(pendingResponseHandler_);
            pendingResponseHandler_ = nullptr;
            h(line);
            return;
        }
    }

    // 2) PROJECT_UPDATE
    if (line.rfind("PROJECT_UPDATE ", 0) == 0 && onProjectUpdateCallback_) {
        // PROJECT_UPDATE id json...
        auto p1 = line.find(' ');
        auto p2 = line.find(' ', p1 + 1);
        auto id = line.substr(p1 + 1, p2 - p1 - 1);
        auto data = line.substr(p2 + 1);
        onProjectUpdateCallback_(id, data);
        return;
    }

    // 3) MODEL_UPDATED
    if (line.rfind("MODEL_UPDATED ", 0) == 0 && onModelUpdateCallback_) {
        auto p1 = line.find(' ');
        auto p2 = line.find(' ', p1 + 1);
        auto id = line.substr(p1 + 1, p2 - p1 - 1);
        auto mp = line.substr(p2 + 1);
        onModelUpdateCallback_(id, mp);
        return;
    }

    // 4) MODEL_DATA
    if (line.rfind("MODEL_DATA ", 0) == 0) {
        // MODEL_DATA pid path base64...
        auto p1 = line.find(' ');
        auto p2 = line.find(' ', p1 + 1);
        auto p3 = line.find(' ', p2 + 1);
        auto id = line.substr(p1 + 1, p2 - p1 - 1);
        auto mp = line.substr(p2 + 1, p3 - p2 - 1);
        auto b64 = line.substr(p3 + 1);
        auto data = base64_decode(b64);

        std::string key = id + ":" + mp;
        std::function<void(const std::vector<char>&)> cb;
        {
            std::lock_guard lock(downloadMutex_);
            auto it = modelDownloadCallbacks_.find(key);
            if (it != modelDownloadCallbacks_.end()) {
                cb = std::move(it->second);
                modelDownloadCallbacks_.erase(it);
            }
        }
        if (cb) cb(data);
        return;
    }

    // 5) CHAT or CHAT_PROJECT or anything else → queue
    {
        std::lock_guard lock(messagesMutex_);
        messageQueue_.push(line);
    }
}

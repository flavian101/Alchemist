#pragma once

#include <string>
#include <vector>
#include "network/NetworkServer.h"
#include <mutex>

class ChatWindow {
public:
    ChatWindow(NetworkServer& server);
    void render(); // Call this in your main render loop
    void addMessage(const std::string& message);

private:
    NetworkServer& server_; // Reference to the NetworkServer
    std::vector<std::string> messages_;
    char inputBuffer_[256]; // Buffer for user input
    std::mutex messagesMutex_;

};

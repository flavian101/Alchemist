#pragma once

#include <string>
#include <vector>
#include <chrono>
#include "Project/Project.h"

class Client;

class ChatWindow {
public:
    ChatWindow(Client& client); 
    void render();
    void addMessage(const std::string& message);
    void saveChatHistory(const std::string& filename);
    void loadChatHistory(const std::string& filename);

private:
    Client& client_;
    std::vector<std::pair<std::string, std::chrono::system_clock::time_point>> messages_;
    char inputBuffer_[256];
    bool isWindowFocused_;
};

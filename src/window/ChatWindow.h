#pragma once

#include <string>
#include <vector>
#include <chrono>
#include "Project/Project.h"
#include <mutex>


class Client;
class Project;


class ChatWindow {
public:
    ChatWindow(Client& client); 
    void render();
    void addMessage(const std::string& message);
	void sendMessage(const std::string& message);
    void saveChatHistory(const std::string& filename);
    void loadChatHistory(const std::string& filename);

    size_t getMessageCount();

private:
    Client& client_;
    std::vector<std::pair<std::string, std::chrono::system_clock::time_point>> messages_;
    char inputBuffer_[256];
    bool isWindowFocused_;
    std::mutex messagesMutex_;

};

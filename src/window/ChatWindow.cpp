#include "ChatWindow.h"
#include "network/Client.h"
#include "imgui/imgui.h"
#include <fstream>
#include <iomanip>
#include <sstream>

ChatWindow::ChatWindow(Client& client)
    : client_(client), isWindowFocused_(true) {
    inputBuffer_[0] = '\0'; // Initialize input buffer
}

void ChatWindow::render() {
    ImGui::Begin("Chat Window", &isWindowFocused_);

    ImGui::BeginChild("ChatMessages", ImVec2(300, 200), true);
    {
        std::lock_guard<std::mutex> lock(messagesMutex_);
        for (const auto& [message, timestamp] : messages_) {
            auto time = std::chrono::system_clock::to_time_t(timestamp);
            struct tm timeInfo;
#ifdef _WIN32
            localtime_s(&timeInfo, &time);
#else
            localtime_r(&time, &timeInfo);
#endif
            std::stringstream timeStream;
            timeStream << std::put_time(&timeInfo, "[%H:%M:%S] ");
            ImGui::TextWrapped("%s%s", timeStream.str().c_str(), message.c_str());
        }
    }
    if (!messages_.empty()) {
        ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();

    bool enterPressed = ImGui::InputText("##Input", inputBuffer_, sizeof(inputBuffer_), ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    bool sendClicked = ImGui::Button("Send");

    if ((enterPressed || sendClicked) && inputBuffer_[0] != '\0') {
        std::string message = inputBuffer_;
        sendMessage(message);
        inputBuffer_[0] = '\0';
        ImGui::SetKeyboardFocusHere(-1);
    }
    ImGui::End();
}

void ChatWindow::addMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(messagesMutex_);
    messages_.emplace_back(message, std::chrono::system_clock::now());
    if (messages_.size() > 100) {
        messages_.erase(messages_.begin());
    }
}

void ChatWindow::sendMessage(const std::string& message)
{
    if (!message.empty()) {
        // Add message to our local chat history
        addMessage("You: " + message);

        // Send to server
        client_.sendMessageToServer(message);
    }
}

void ChatWindow::saveChatHistory(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (const auto& [message, timestamp] : messages_) {
        auto time = std::chrono::system_clock::to_time_t(timestamp);
        struct tm timeInfo;
        localtime_s(&timeInfo, &time); 
        file << std::put_time(&timeInfo, "[%H:%M:%S] ") << message << "\n";
    }
}

void ChatWindow::loadChatHistory(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    messages_.clear();
    std::string line;
    while (std::getline(file, line)) {
        messages_.emplace_back(line, std::chrono::system_clock::now());
    }
}

size_t ChatWindow::getMessageCount()  {
    std::lock_guard<std::mutex> lock(messagesMutex_);
    return messages_.size();
}

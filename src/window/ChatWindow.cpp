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
    // Create a window for the chat
    ImGui::Begin("Chat Window", &isWindowFocused_);

    // Display chat messages
    ImGui::BeginChild("ChatMessages", ImVec2(0, -50), true);
    for (const auto& [message, timestamp] : messages_) {
        // Format timestamp
        auto time = std::chrono::system_clock::to_time_t(timestamp);
        struct tm timeInfo;
        localtime_s(&timeInfo, &time);
        std::stringstream timeStream;
        timeStream << std::put_time(&timeInfo, "[%H:%M:%S] ");
        ImGui::TextWrapped("%s%s", timeStream.str().c_str(), message.c_str());
    }
    ImGui::EndChild();

    // Input field and send button
    ImGui::InputText("##Input", inputBuffer_, sizeof(inputBuffer_));
    ImGui::SameLine();
    if (ImGui::Button("Send")) {
        if (inputBuffer_[0] != '\0') {
            std::string message = inputBuffer_;
            addMessage("You: " + message); // Add the user's message to the chat window
            client_.sendMessageToServer(message); // Send the message via the Client
            inputBuffer_[0] = '\0'; // Clear the input buffer
        }
    }


    ImGui::End();
}

void ChatWindow::addMessage(const std::string& message) {
    messages_.emplace_back(message, std::chrono::system_clock::now());
    // Optional: Limit the number of messages to avoid excessive memory usage
    if (messages_.size() > 100) {
        messages_.erase(messages_.begin());
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

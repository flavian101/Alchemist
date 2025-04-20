#include "ChatWindow.h"
#include "imgui/imgui.h"

ChatWindow::ChatWindow(NetworkServer& server)
    : server_(server) {
    inputBuffer_[0] = '\0'; // Initialize input buffer
}

void ChatWindow::render() {
    // Create a window for the chat
    ImGui::Begin("Chat Window");

    // Display chat messages
    ImGui::BeginChild("ChatMessages", ImVec2(0, -30), true);
    for (const auto& message : messages_) {
        ImGui::TextWrapped("%s", message.c_str());
    }
    ImGui::EndChild();

    // Input field and send button
    ImGui::InputText("##Input", inputBuffer_, sizeof(inputBuffer_));
    ImGui::SameLine();
    if (ImGui::Button("Send")) {
        if (inputBuffer_[0] != '\0') {
            std::string message = inputBuffer_;
            addMessage("You: " + message); // Show locally 

            // Format for network transmission - prefix with CHAT
            std::string networkMessage = "CHAT " + message;
            server_.processChatMessage(networkMessage);

            inputBuffer_[0] = '\0'; // Clear input
        }
    }

    ImGui::End();
}


void ChatWindow::addMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(messagesMutex_); // Add mutex member variable
    messages_.push_back(message);
    if (messages_.size() > 100) {
        messages_.erase(messages_.begin());
    }
}

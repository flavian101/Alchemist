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
            std::string taggedMessage = "ClientName: " + message; // Append the client's name
            addMessage("You: " + message); // Add the user's message to the chat window
            server_.processChatMessage(taggedMessage, nullptr); // Use processChatMessage() instead
            inputBuffer_[0] = '\0'; // Clear the input buffer
        }
    }

    ImGui::End();
}


void ChatWindow::addMessage(const std::string& message) {
    messages_.push_back(message);
    // Optional: Limit the number of messages to avoid excessive memory usage
    if (messages_.size() > 100) {
        messages_.erase(messages_.begin());
    }
}

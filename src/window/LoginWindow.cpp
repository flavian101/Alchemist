#include "LoginWindow.h"
#include <iostream>

LoginWindow::LoginWindow(Window& win, Client& client)
    : m_window(win), m_client(client) {
}

bool LoginWindow::Show() {
    bool result = false;

    if (!loggedIn) { // Show login UI only if not already logged in
        ImGui::Begin("Login");

        ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
        ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

        if (ImGui::Button("Login")) {
            // Use the Client class to authenticate
            if (m_client.connectToServer("127.0.0.1", "12345") &&
                m_client.authenticate(username, password)) {
                loggedIn = true;
                ShowMessage("Successfully Logged In!", ImVec4(0.0f, 1.0f, 0.0f, 1.0f), true);
                result = true; // Indicate success this frame
            }
            else {
                ShowMessage("Invalid username or password. Try again.", ImVec4(1.0f, 0.0f, 0.0f, 1.0f), false);
            }
        }

        ImGui::End();
    }

    return result;
}

std::string LoginWindow::GetUsername() const {
    return std::string(username);
}

std::string LoginWindow::GetPassword() const {
    return std::string(password);
}

void LoginWindow::ShowMessage(const std::string& message, const ImVec4& color, bool isSuccess) {
    auto now = std::chrono::steady_clock::now();
    auto& flag = isSuccess ? showSuccessMessage : showErrorMessage;
    auto& timePoint = isSuccess ? successMessageTime : errorMessageTime;

    flag = true;
    timePoint = now;

    if (flag) {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - timePoint);
        if (duration.count() < 10) {
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.8f);
            if (ImGui::Begin(isSuccess ? "Success" : "Error", nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
                ImGui::TextColored(color, "%s", message.c_str());
                ImGui::End();
            }
        }
        else {
            flag = false;
        }
    }
}

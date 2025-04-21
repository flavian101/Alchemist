#include "LoginWindow.h"
#include <iostream>

LoginWindow::LoginWindow(Window& win, Client& client)
    : m_window(win), m_client(client) {
    username[0] = '\0';
    password[0] = '\0';
}

bool LoginWindow::Show() {
    bool result = false;

    ImGui::Begin("Login / Register");

    static bool showRegistration = false;

    ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
    ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

    if (!showRegistration) {
        // Login Section
        if (ImGui::Button("Login")) {
            if (m_client.connectToServer("127.0.0.1", "12345") &&
                m_client.authenticate(username, password)) {
                loggedIn = true;
                ShowMessage("Successfully Logged In!", ImVec4(0.0f, 1.0f, 0.0f, 1.0f), true);
                result = true;
            }
            else {
                ShowMessage("Invalid username or password.", ImVec4(1.0f, 0.0f, 0.0f, 1.0f), false);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Create Account")) {
            showRegistration = true;
        }
    }
    else {
        // Registration Section
        if (ImGui::Button("Register")) {
            if (m_client.connectToServer("127.0.0.1", "12345") &&
                m_client.registerUser(username, password)) {
                ShowMessage("Registration successful!", ImVec4(0.0f, 1.0f, 0.0f, 1.0f), true);
                showRegistration = false;
            }
            else {
                ShowMessage("Registration failed. Username may already exist.", ImVec4(1.0f, 0.0f, 0.0f, 1.0f), false);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Back to Login")) {
            showRegistration = false;
        }
    }

    ImGui::End();
    RenderToasts();
    // Render popup messages
    ShowActiveMessages();

    return result;
}

std::string LoginWindow::GetUsername() const {
    return std::string(username);
}

std::string LoginWindow::GetPassword() const {
    return std::string(password);
}

void LoginWindow::ShowMessage(const std::string& message, const ImVec4& color, bool isSuccess) {
    AddToast(message, color, 3.0f); // 3-second toast
}

void LoginWindow::AddToast(const std::string& message, const ImVec4& color, float duration)
{
    toasts.push_back({ message, color, std::chrono::steady_clock::now(), duration });

}

void LoginWindow::RenderToasts()
{
    const float padding = 10.0f;
    const float toastWidth = 300.0f;
    ImVec2 startPos = ImVec2(ImGui::GetIO().DisplaySize.x - toastWidth - padding, padding);

    for (size_t i = 0; i < toasts.size(); ) {
        const auto& toast = toasts[i];
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration<float>(now - toast.timestamp).count();

        if (elapsed > toast.duration) {
            toasts.erase(toasts.begin() + i); // remove expired toast
            continue;
        }

        float alpha = 1.0f;
        if (elapsed > toast.duration - 1.0f) {
            alpha = toast.duration - elapsed; // fade out
        }

        ImGui::SetNextWindowBgAlpha(alpha * 0.85f);
        ImGui::SetNextWindowPos(ImVec2(startPos.x, startPos.y + i * 60.0f), ImGuiCond_Always);

        ImGui::Begin(("##toast_" + std::to_string(i)).c_str(), nullptr,
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav);

        ImGui::TextColored(toast.color, "%s", toast.message.c_str());
        ImGui::End();

        ++i;
    }
}

void LoginWindow::ShowActiveMessages() {
    auto now = std::chrono::steady_clock::now();

    if (showSuccessMessage) {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - successMessageTime);
        if (duration.count() < 10) {
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.8f);
            if (ImGui::Begin("Success", nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
                ImGui::TextColored(successColor, "%s", successMessage.c_str());
                ImGui::End();
            }
        }
        else {
            showSuccessMessage = false;
        }
    }

    if (showErrorMessage) {
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - errorMessageTime);
        if (duration.count() < 10) {
            ImGui::SetNextWindowPos(ImVec2(10, 60), ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.8f);
            if (ImGui::Begin("Error", nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
                ImGui::TextColored(errorColor, "%s", errorMessage.c_str());
                ImGui::End();
            }
        }
        else {
            showErrorMessage = false;
        }
    }
}

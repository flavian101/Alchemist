#include "LoginWindow.h"
#include <iostream>

LoginWindow::LoginWindow(Window& win) : m_window(win) {}

bool LoginWindow::Show()
{
    bool result = false;

    if (!loggedIn)  // Show login UI only if not already logged in
    {
        ImGui::Begin("Login");

        ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
        ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

        if (ImGui::Button("Login")) {
            loggedIn = Authenticate(username, password); // <-- use this flag
            if (loggedIn) {
                showSuccessMessage = true;
                successMessageTime = std::chrono::steady_clock::now();
                result = true; // Indicate success this frame
            }
            else {
                showErrorMessage = true;
                errorMessageTime = std::chrono::steady_clock::now();
            }
        }

        if (showErrorMessage) {
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - errorMessageTime);

            if (duration.count() < 3) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Invalid username or password. Try again.");
            }
            else {
                showErrorMessage = false;
            }
        }

        ImGui::End();
    }

    // This part will show even after the login window is hidden
    if (showSuccessMessage) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - successMessageTime);

        if (duration.count() < 3) {
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(0.8f);
            if (ImGui::Begin("Success", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
                ImGui::Text("Successfully Logged In!");
                ImGui::End();
            }
        }
        else {
            showSuccessMessage = false;
        }
    }

    return result;
}

bool LoginWindow::Authenticate(const std::string& username, const std::string& password) {
    // Implement your authentication logic here
    // For simplicity, let's assume any non-empty username and password are valid
    return !username.empty() && !password.empty();
}

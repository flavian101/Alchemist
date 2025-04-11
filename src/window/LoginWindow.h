#pragma once

#include "window/Window.h"
#include "network/Client.h"
#include <imgui/imgui.h>
#include <string>
#include <chrono>

class LoginWindow {
public:
    LoginWindow(Window& win, Client& client);
    bool Show(); // Displays the login window and handles user input
    std::string GetUsername() const;
    std::string GetPassword() const;

private:
    Window& m_window;
    Client& m_client; // Reference to the Client class for authentication
    char username[256] = ""; // Buffer for username input
    char password[256] = ""; // Buffer for password input
    bool loggedIn = false;  // Keep track of the login state locally for this window

    // Error and success message handling
    bool showErrorMessage = false;
    std::chrono::steady_clock::time_point errorMessageTime;
    bool showSuccessMessage = false;
    std::chrono::steady_clock::time_point successMessageTime;

    void ShowMessage(const std::string& message, const ImVec4& color, bool isSuccess);
};

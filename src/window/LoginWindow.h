#pragma once
#include "window/Window.h"
#include <imgui/imgui.h>
#include <string>
#include <chrono>


class LoginWindow {
public:
    LoginWindow(Window& win);
    bool Show();

private:
    Window& m_window;
    bool Authenticate(const std::string& username, const std::string& password);
    char username[128] = "";
    char password[128] = "";
    bool loginSuccess = false;
	bool loggedIn = false;

    bool showErrorMessage = false;
    std::chrono::steady_clock::time_point errorMessageTime;

    // New members for success message
    bool showSuccessMessage = false;
    std::chrono::steady_clock::time_point successMessageTime;
};

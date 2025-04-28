#include "ClientApp.h"
#include <iostream>
#include <string>
#include <memory>

ClientApp::ClientApp()
    : window(L"engine", L"DirectX", 1366, 768),
    imgui(),
    client(),
    loginWin(window, client),
    loggedIn(false) {
}

int ClientApp::createLoop() {
    while (true) {
        // Process all messages pending  
        if (const auto ecode = Window::ProcessMessages()) {
            // If return optional has value, means we're exiting the program by returning the exit code
            return *ecode;
        }

        window.GetInstance().ClearDepthColor(0.0f, 0.0f, 0.0f);

        // Handle login state
        if (!loggedIn) {
            // Use LoginWindow for authentication
            if (loginWin.Show()) {
                loggedIn = true;

                projectManager = std::make_unique<ClientProjectManager>(window, &client);

                // Transfer authentication data to project manager
                projectManager->SetLoggedIn(true);
                projectManager->SetCurrentUsername(loginWin.GetUsername());
                // Load projects after successful login
                projectManager->LoadProjectsFromServer();
            }
            window.GetInstance().End();
            continue;
        }
        
        Render();
    }
}

void ClientApp::Render() {
    // Show menu bar but don't handle login there
    projectManager->ShowMenuBar(window.GetInstance());

    if (loggedIn) {
    

        if (projectManager->HasCurrentProject()) {
            projectManager->ShowChatWindow();
        }

        // Process any pending messages from the client
        client.processMessages();
        projectManager->Update(window.GetInstance());
        projectManager->Render(window.GetInstance());
    }

    window.GetInstance().End();
}

ClientApp::~ClientApp() {
}

void ClientApp::GameProjects() {
}
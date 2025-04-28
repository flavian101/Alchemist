#include "ServerApp.h"
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

ServerApp::ServerApp()
    :
    window(L"engine", L"DirectX", 1366, 768),
    imgui(),
    ssl_context(boost::asio::ssl::context::sslv23),
    dbManager(nullptr),
    server(nullptr),
    chatWindow(nullptr) ,
    manager(nullptr)
{
}


int ServerApp::createLoop()
{
    while (true)
    {
        //process all messages pending 
        if (const auto ecode = Window::ProcessMessages())
        {
            //if return optional has value, means we'are exiting the program by returning the exit code
            return *ecode;
        }

        window.GetInstance().ClearDepthColor(0.0f, 0.0f, 0.0f);

       
        Render();

       
    }
 
}
void ServerApp::Render()
{

    ShowServerWindow();
    if (chatWindow) {
        chatWindow->render(); // Render the chat window
    }

    if (manager) {
        manager->ShowMenuBar(window.GetInstance());
        manager->ShowProjectWindow(); // This is the Admin view
        manager->Update(window.GetInstance());
        manager->Render(window.GetInstance());
    }
    window.GetInstance().End();


}

void ServerApp::StartServer()
{
    try {
        ssl_context.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::single_dh_use);
        ssl_context.use_certificate_chain_file("server.crt");
        ssl_context.use_private_key_file("server.key", boost::asio::ssl::context::pem);

        dbManager = new DatabaseManager("database.db");
        server = std::make_shared<NetworkServer>(io_context, ssl_context, 12345, *dbManager); // Port 12345
        manager = std::make_unique<ServerProjectManager>(window, *dbManager,server.get());

        // Create the chat window and pass the server reference
        chatWindow = new ChatWindow(*server);

        // Set the callback for incoming messages
        server->setMessageReceivedCallback([this](const std::string& message) {
            // Route message to chat window
            if (chatWindow) {
                // Only show chat messages in the chat window
                if (message.find("CHAT ") == 0) {
                    chatWindow->addMessage(message.substr(5)); // Remove "CHAT " prefix
                }
                else if (message.find("CHAT_PROJECT ") == 0) {
                    // Extract project ID and parse message
                    size_t firstSpace = message.find(' ', 13); // 13 is length of "CHAT_PROJECT "
                    if (firstSpace != std::string::npos) {
                        std::string projectId = message.substr(13, firstSpace - 13);
                        std::string chatMessage = message.substr(firstSpace + 1);
                        chatWindow->addMessage("[Project " + projectId + "] " + chatMessage);
                    }
                }
            }

            // Log all message types to server logs
            if (message.find("SAVE_PROJECT") == 0 ||
                message.find("LOAD_PROJECT") == 0 ||
                message.find("LIST_PROJECTS") == 0) {
                AddLog("Project operation: " + message);
            }
            else if (message.find("AUTH ") == 0) {
                // Don't log passwords
                AddLog("Authentication attempt from client");
            }
            else if (message.find("REGISTER ") == 0) {
                // Don't log passwords
                AddLog("Registration attempt from client");
            }
            else {
                // Log other messages
                AddLog("Message received: " + message);
            }
            });

        serverRunning = true;
        io_context.restart();

        // Run IO context in a separate thread
        serverThread = std::thread([this]() {
            try {
                AddLog("IO context thread started");
                io_context.run();
                AddLog("IO context thread stopped");
            }
            catch (const std::exception& e) {
                AddLog("IO thread error: " + std::string(e.what()));
                serverRunning = false;
            }
            });
    }
    catch (std::exception& e) {
        AddLog("Server startup error: " + std::string(e.what()));
        serverRunning = false;
    }
}

void ServerApp::StopServer()
{
    if (serverRunning) {
        // Notify all clients that server is shutting down
        if (server) {
            server->BroadcastChatMessage("SERVER_SHUTDOWN Server is shutting down. Please save your work.");
            // Give clients time to process the message
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        io_context.stop();
        if (serverThread.joinable()) {
            serverThread.join();
        }

        delete dbManager;
        dbManager = nullptr;
        server.reset();
        serverRunning = false;

        AddLog("Server stopped");
    }
}

void ServerApp::ShowServerWindow() {
    ImGui::Begin("Server Control");

    // Server status
    if (serverRunning) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Server Status: Running");
    }
    else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Server Status: Stopped");
    }

    // Start/Stop buttons
    if (!serverRunning) {
        if (ImGui::Button("Start Server")) {
            AddLog("Starting server...");
            StartServer();
        }
    }
    else {
        if (ImGui::Button("Stop Server")) {
            AddLog("Stopping server...");
            StopServer();
        }
    }

    // Server metrics (if running)
    if (serverRunning && server) {
        ImGui::Separator();
        ImGui::Text("Server Metrics:");
       // ImGui::Text("Connected Clients: %d", server->getClientCount());
        ImGui::Text("Projects: %d", manager ? manager->GetProjectCount() : 0);
    }

    // Logs
    ImGui::Separator();
    ImGui::Text("Server Logs:");

    ImGui::BeginChild("LogsScrollRegion", ImVec2(0, 200), true);
    {
        std::lock_guard<std::mutex> lock(logMutex);
        for (const auto& log : serverLogs) {
            ImGui::TextWrapped("%s", log.c_str());
        }
    }

    // Auto-scroll to bottom
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 20) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::EndChild();

    // Log controls
    if (ImGui::Button("Clear Logs")) {
        std::lock_guard<std::mutex> lock(logMutex);
        serverLogs.clear();
    }

    ImGui::End();
}

void ServerApp::AddLog(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    // Add timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&time_t_now), "%H:%M:%S") << "] " << message;

    serverLogs.push_back(ss.str());

    // Limit log size
    const size_t MAX_LOGS = 100;
    if (serverLogs.size() > MAX_LOGS) {
        serverLogs.erase(serverLogs.begin());
    }
}

ServerApp::~ServerApp()
{
    StopServer(); // Ensure the server is stopped
    delete chatWindow; // Clean up the chat window
}


void ServerApp::GameProjects()
{
    
}
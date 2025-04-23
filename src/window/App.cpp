#include "App.h"
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

App::App()
    :
    window(L"engine", L"DirectX", 1366, 768),
    imgui(),
    ssl_context(boost::asio::ssl::context::sslv23),
    dbManager(nullptr),
    server(nullptr),
    chatWindow(nullptr) ,
    manager(window)
{
}


int App::createLoop()
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
void App::Render()
{

    ShowServerWindow();
    if (chatWindow) {
        chatWindow->render(); // Render the chat window
    }
    window.GetInstance().End();


}

void App::StartServer()
{
    try {
        ssl_context.set_options(boost::asio::ssl::context::default_workarounds | boost::asio::ssl::context::no_sslv2 | boost::asio::ssl::context::single_dh_use);
        ssl_context.use_certificate_chain_file("server.crt");
        ssl_context.use_private_key_file("server.key", boost::asio::ssl::context::pem);

        dbManager = new DatabaseManager("database.db");
        server = std::make_shared<NetworkServer>(io_context, ssl_context, 12345, *dbManager); // Port 12345

        // Create the chat window and pass the server reference
        chatWindow = new ChatWindow(*server);

        // Set the callback for incoming messages
        server->setMessageReceivedCallback([this](const std::string& message) {
            if (chatWindow) {
                chatWindow->addMessage("Server: " + message);
            }
            });

        serverRunning = true;
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        serverRunning = false;
    }
}

void App::StopServer()
{
    if (serverRunning) {
        io_context.stop();
        if (serverThread.joinable()) {
            serverThread.join();
        }
        delete dbManager;
        serverRunning = false;
    }
}

void App::ShowServerWindow()
{
    ImGui::Begin("Server Control");

    // Server status
    if (serverRunning) {
        ImGui::Text("Server Status: Running");
    }
    else {
        ImGui::Text("Server Status: Stopped");
    }

    // Start/Stop buttons
    if (!serverRunning) {
        if (ImGui::Button("Start Server")) {
            serverThread = std::thread(&App::StartServer, this);
        }
    }
    else {
        if (ImGui::Button("Stop Server")) {
            StopServer();
        }
    }

    // Logs (placeholder for now)
    ImGui::Separator();
    ImGui::Text("Logs:");
    ImGui::TextWrapped("This is where server logs will appear.");

    ImGui::End();
}


App::~App()
{
    StopServer(); // Ensure the server is stopped
    delete chatWindow; // Clean up the chat window
}


void App::GameProjects()
{
    
}
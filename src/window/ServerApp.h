#pragma once
#include "Window.h"
#include "ImguiManager.h"
#include "Graphics/FrameTime.h"
#include <vector>
#include "Graphics/Graphics.h"
#include <memory>
#include "network/NetworkServer.h"
#include "network/DatabaseManager.h"
#include <stdexcept>
#include <iostream>
#include <boost/asio/ssl.hpp>
#include "window/ChatWindow.h"
#include "Project/ServerProjectManager.h"

class ServerApp
{
public:
	ServerApp();
	~ServerApp();
	int createLoop();
	void StartServer();
	void StopServer();
	void ShowServerWindow();

	void AddLog(const std::string& message);


private:
	void Render();
	void GameProjects();
	//
private:
	ImguiManager imgui;
	Window window;
	
	float speedFactor = 1.0f;
	bool serverRunning = false;
	std::thread serverThread;
	boost::asio::io_context io_context;
	boost::asio::ssl::context ssl_context;
	DatabaseManager* dbManager = nullptr;
	std::shared_ptr<NetworkServer> server = nullptr;
	ChatWindow* chatWindow = nullptr;
	std::unique_ptr<ServerProjectManager> manager;

	std::vector<std::string> serverLogs;
	std::mutex logMutex;

	
};


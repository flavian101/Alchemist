#include "NetworkServer.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <thread>
#include <algorithm>
#include <mutex>
#include <ctime>
#include <filesystem> 
#include <fstream>
#include <unordered_set>

namespace fs = std::filesystem; 

NetworkServer::NetworkServer(boost::asio::io_context& io_context, boost::asio::ssl::context& ssl_context, short port, DatabaseManager& dbManager)
    : io_context_(io_context),
    ssl_context_(ssl_context),
    acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    dbManager_(dbManager)
{
    startAccept();
}

void NetworkServer::startAccept() {
    auto socket = std::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>(io_context_, ssl_context_);
    acceptor_.async_accept(socket->lowest_layer(), [this, socket](const boost::system::error_code& error) {
        handleAccept(error, socket);
        startAccept(); // Continue accepting new clients
        });
}

void NetworkServer::handleAccept(const boost::system::error_code& error, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket) {
    if (!error) {
        std::cout << "New client connected!" << std::endl;
        socket->async_handshake(boost::asio::ssl::stream_base::server, [this, socket](const boost::system::error_code& error) {
            if (!error) {
                std::lock_guard lock(clientMutex_);
                clients_.push_back(socket);         // ← move it *inside* the success branch
                handleClient(socket);
            }
            else {
                std::cerr << "Handshake error: " << error.message() << "\n";
            }
            });
    }
}

void NetworkServer::handleClient(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket) {
    auto buffer = std::make_shared<boost::asio::streambuf>();

    boost::asio::async_read_until(*socket, *buffer, "\n",
        [this, socket, buffer](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            if (!ec) {
                std::istream is(buffer.get());
                std::string message;
                std::getline(is, message);

                handleIncomingMessage(socket, message);

                // Continue listening for next message
                handleClient(socket);
            }
            else {
                handleClientDisconnect(socket, ec);
            }
        });
}

void NetworkServer::authenticateClient(const std::string& username,
    const std::string& password,
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket)
{
    if (dbManager_.authenticateUser(username, password)) {
        {
            std::lock_guard<std::mutex> lock(clientMutex_);
            authenticatedClients_[socket] = true;
            authenticatedUsernames_[socket] = username;
        }
        std::string successMessage = "Authentication successful.\n";
        boost::asio::async_write(*socket, boost::asio::buffer(successMessage),
            [](const boost::system::error_code& ec, std::size_t) {
                if (ec) {
                    std::cerr << "Error sending success message: " << ec.message() << std::endl;
                }
            });
    }
    else {
        std::string failureMessage = "Authentication failed.\n";
        boost::asio::async_write(*socket, boost::asio::buffer(failureMessage),
            [socket](const boost::system::error_code& ec, std::size_t) {
                if (!ec) {
                    socket->async_shutdown([socket](const boost::system::error_code& shutdown_ec) {
                        if (shutdown_ec) {
                            std::cerr << "Shutdown error: " << shutdown_ec.message() << std::endl;
                        }
                        else {
                            std::cout << "Client disconnected cleanly after auth failure.\n";
                        }
                        });
                }
                else {
                    std::cerr << "Error sending failure message: " << ec.message() << std::endl;
                }
            });
    }
}

void NetworkServer::RegisterUser(const std::string& username, const std::string& password, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket)
{
    if (dbManager_.registerUser(username, password)) {
        std::string successMessage = "Registration successful.\n";
        boost::asio::async_write(*socket, boost::asio::buffer(successMessage),
            [](const boost::system::error_code& ec, std::size_t) {
                if (ec) {
                    std::cerr << "Error sending success message: " << ec.message() << std::endl;
                }
            });
    }
    else {
        std::string failureMessage = "Registration failed. Username may already exist.\n";
        boost::asio::async_write(*socket, boost::asio::buffer(failureMessage),
            [](const boost::system::error_code& ec, std::size_t) {
                if (ec) {
                    std::cerr << "Error sending failure message: " << ec.message() << std::endl;
                }
            });
    }
}

void NetworkServer::processChatMessage(const std::string& message, std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> sender) {
    // Verify authentication
    {
        std::lock_guard<std::mutex> lock(clientMutex_);
        if (authenticatedClients_.find(sender) == authenticatedClients_.end() || !authenticatedClients_[sender]) {
            std::string errorMsg = "You must be authenticated to send messages.\n";
            boost::asio::async_write(*sender, boost::asio::buffer(errorMsg),
                [](const boost::system::error_code& ec, std::size_t) {
                    if (ec) {
                        std::cerr << "Error sending error message: " << ec.message() << std::endl;
                    }
                });
            return;
        }
    }

    // Extract message content
    std::string msgContent;
    if (message.find("CHAT ") == 0) {
        msgContent = message.substr(5);
    }
    else {
        msgContent = message;
    }

    // Add timestamp
    std::time_t currentTime = std::time(nullptr);
    std::string timeStr = std::ctime(&currentTime);
    timeStr.pop_back(); // Remove trailing newline

    // Format final message with CHAT prefix
    std::string formattedMessage = "CHAT [" + timeStr + "] " + msgContent;

    // Broadcast to all clients
    BroadcastChatMessage(formattedMessage);

    // Callback if registered
    if (messageReceivedCallback_) {
        messageReceivedCallback_(msgContent);
    }
}

void NetworkServer::processChatMessage(const std::string& message) {
    std::string msgContent = (message.find("CHAT ") == 0) ? message.substr(5) : message;
    std::time_t currentTime = std::time(nullptr);
    std::string timeStr = std::ctime(&currentTime);
    timeStr.pop_back();
    std::string formattedMessage = "CHAT [" + timeStr + "] " + msgContent;
    BroadcastChatMessage(formattedMessage);

    if (messageReceivedCallback_) {
        messageReceivedCallback_(msgContent);
    }
}

void NetworkServer::BroadcastChatMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(clientMutex_);
    for (auto it = clients_.begin(); it != clients_.end();) {
        auto client = *it;
        if (authenticatedClients_.find(client) != authenticatedClients_.end() && authenticatedClients_[client]) {
            boost::asio::async_write(*client, boost::asio::buffer(message + "\n"),
                [this, client](const boost::system::error_code& error, std::size_t) {
                    if (error) {
                        std::cerr << "Error sending message to client: " << error.message() << std::endl;
                        std::lock_guard<std::mutex> lock(clientMutex_);
                        authenticatedClients_.erase(client);
                        clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
                    }
                });
            ++it;
        }
        else {
            it = clients_.erase(it);
        }
    }
}

void NetworkServer::setMessageReceivedCallback(const std::function<void(const std::string&)>& callback) {
    messageReceivedCallback_ = callback;
}
void NetworkServer::handleIncomingMessage(
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket,
    const std::string& message) {

    std::cout << "Received: " << message << std::endl;

    if (message.find("AUTH ") == 0) {
        std::string credentials = message.substr(5);
        size_t spacePos = credentials.find(' ');
        if (spacePos != std::string::npos) {
            std::string username = credentials.substr(0, spacePos);
            std::string password = credentials.substr(spacePos + 1);
            password.erase(password.find_last_not_of(" \r\n") + 1);
            authenticateClient(username, password, socket);
        }
    }
    else if (message.find("REGISTER ") == 0) {
        std::string credentials = message.substr(9);
        size_t spacePos = credentials.find(' ');
        if (spacePos != std::string::npos) {
            std::string username = credentials.substr(0, spacePos);
            std::string password = credentials.substr(spacePos + 1);
            password.erase(password.find_last_not_of(" \r\n") + 1);
            RegisterUser(username, password, socket);
        }
    }
    else if (message.find("SAVE_PROJECT ") == 0) {
        handleSaveProject(socket, message.substr(13));
    }
    else if (message.find("LOAD_PROJECT ") == 0) {
        handleLoadProject(socket, message.substr(13));
    }
    else if (message.find("LIST_PROJECTS ") == 0) {
        handleListProjects(socket, message.substr(14));
    }
    else if (message.find("PUSH_PROJECT ") == 0) {
        handlePushProject(socket, message.substr(13));
    }
    else if (message.find("CHAT_PROJECT ") == 0) {
        // Handle project-specific chat
        handleProjectChat(socket, message.substr(13)); // 13 is length of "CHAT_PROJECT "
    }
    else if (message.find("UPLOAD_MODEL ") == 0) {
        handleModelUpload(socket, message.substr(13)); // 13 is length of "UPLOAD_MODEL "
    }
    else if (message.find("DOWNLOAD_MODEL ") == 0) {
        handleModelDownload(socket, message.substr(15)); // 15 is length of "DOWNLOAD_MODEL "
    }
    else {
        processChatMessage(message, socket);
    }
}
void NetworkServer::handleClientDisconnect(
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket,
    const boost::system::error_code& ec) {

    std::cerr << "Client disconnected: " << ec.message() << std::endl;

    boost::system::error_code ignored_ec;
    socket->shutdown(ignored_ec);  // Clean shutdown

    std::lock_guard<std::mutex> lock(clientMutex_);
    authenticatedClients_.erase(socket);
    authenticatedUsernames_.erase(socket); // Prevent memory leaks / stale entries
    clients_.erase(std::remove(clients_.begin(), clients_.end(), socket), clients_.end());
}

void NetworkServer::handleSaveProject(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket, const std::string& content) {
    size_t first = content.find(' ');
    size_t second = content.find(' ', first + 1);

    if (first == std::string::npos || second == std::string::npos) {
        boost::asio::async_write(*socket, boost::asio::buffer("SAVE_FAILED Invalid format\n"), [](auto, auto) {});
        return;
    }

    std::string projectId = content.substr(0, first);
    std::string name = content.substr(first + 1, second - first - 1);
    std::string jsonData = content.substr(second + 1);

    std::string username = authenticatedUsernames_[socket]; // Safely guarded

    if (dbManager_.storeProject(projectId, name, username, jsonData)) {
        boost::asio::async_write(*socket, boost::asio::buffer("SAVE_OK\n"), [](auto, auto) {});

        // Also broadcast the update to all collaborators
        BroadcastToCollaborators(projectId, "PROJECT_UPDATE " + projectId + " " + jsonData + "\n");
    }
    else {
        boost::asio::async_write(*socket, boost::asio::buffer("SAVE_FAILED\n"), [](auto, auto) {});
    }
}
void NetworkServer::handleLoadProject(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket, const std::string& projectId) {
    auto result = dbManager_.loadProject(projectId);
    if (result.has_value()) {
        std::string response = "PROJECT_DATA " + projectId + " " + result.value() + "\n";
        boost::asio::async_write(*socket, boost::asio::buffer(response), [](auto, auto) {});
    }
    else {
        boost::asio::async_write(*socket, boost::asio::buffer("PROJECT_NOT_FOUND\n"), [](auto, auto) {});
    }
}
void NetworkServer::handleListProjects(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket, const std::string& username) {
    std::cout << "Handling LIST_PROJECTS request for user: " << username << std::endl;

    // Verify the user exists and is authenticated
    {
        std::lock_guard<std::mutex> lock(clientMutex_);
        if (authenticatedClients_.find(socket) == authenticatedClients_.end() || !authenticatedClients_[socket]) {
            std::string errorMsg = "You must be authenticated to list projects.\n";
            boost::asio::async_write(*socket, boost::asio::buffer(errorMsg),
                [](const boost::system::error_code& ec, std::size_t) {
                    if (ec) {
                        std::cerr << "Error sending error message: " << ec.message() << std::endl;
                    }
                });
            return;
        }
    }

    // Get projects for the user
    auto projects = dbManager_.listProjectsForUser(username);
    std::cout << "Found " << projects.size() << " projects for user: " << username << std::endl;

    // Format the response
    std::ostringstream oss;
    oss << "PROJECT_LIST ";
    for (const auto& [id, name] : projects) {
        std::cout << "  Project: " << name << " (ID: " << id << ")" << std::endl;
        oss << id << ":" << name << "|";
    }
    oss << "\n";

    std::string response = oss.str();
    std::cout << "Sending response: \"" << response << "\"" << std::endl;

    // Send the response
    boost::asio::async_write(*socket, boost::asio::buffer(response),
        [response](const boost::system::error_code& ec, std::size_t) {
            if (ec) {
                std::cerr << "Error sending project list: " << ec.message() << std::endl;
            }
            else {
                std::cout << "Successfully sent project list response" << std::endl;
            }
        });
}
void NetworkServer::handlePushProject(std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket, const std::string& content) {
    size_t first = content.find(' ');
    size_t second = content.find(' ', first + 1);

    if (first == std::string::npos || second == std::string::npos) {
        boost::asio::async_write(*socket, boost::asio::buffer("PUSH_FAILED Invalid format\n"), [](auto, auto) {});
        return;
    }

    std::string projectId = content.substr(0, first);
    std::string name = content.substr(first + 1, second - first - 1);
    std::string jsonData = content.substr(second + 1);

    std::string username = authenticatedUsernames_[socket]; // Safely guarded

    if (dbManager_.storeProject(projectId, name, username, jsonData)) {
        std::string updateMessage = "PROJECT_UPDATE " + projectId + " " + jsonData + "\n";

        // Broadcast to all collaborators except the sender
        BroadcastToCollaborators(projectId, updateMessage, socket);

        boost::asio::async_write(*socket, boost::asio::buffer("PUSH_OK\n"), [](auto, auto) {});
    }
    else {
        boost::asio::async_write(*socket, boost::asio::buffer("PUSH_FAILED\n"), [](auto, auto) {});
    }
}
void NetworkServer::handleProjectChat(
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket,
    const std::string& content) {

    // Verify authentication
    std::string username;
    {
        std::lock_guard<std::mutex> lock(clientMutex_);
        if (authenticatedClients_.find(socket) == authenticatedClients_.end() || !authenticatedClients_[socket]) {
            std::string errorMsg = "You must be authenticated to send messages.\n";
            boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
            return;
        }
        username = authenticatedUsernames_[socket];
    }

    // Extract project ID and message
    size_t firstSpace = content.find(' ');
    if (firstSpace == std::string::npos) {
        return; // Invalid format
    }

    std::string projectId = content.substr(0, firstSpace);
    std::string msgContent = content.substr(firstSpace + 1);

    // Verify project access
    std::vector<std::tuple<std::string, std::string>> collaborators = dbManager_.getCollaboratorsForProject(projectId);
    bool hasAccess = false;

    for (const auto& [collab, role] : collaborators) {
        if (collab == username) {
            hasAccess = true;
            break;
        }
    }

    if (!hasAccess) {
        std::string errorMsg = "You don't have access to this project.\n";
        boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
        return;
    }

    // Format and broadcast the message to all collaborators of this project
    std::time_t currentTime = std::time(nullptr);
    std::string timeStr = std::ctime(&currentTime);
    timeStr.pop_back(); // Remove trailing newline

    std::string formattedMessage = "CHAT_PROJECT " + projectId + " [" + timeStr + "] " + username + ": " + msgContent;

    // Broadcast to all clients with access to this project
    std::lock_guard<std::mutex> lock(clientMutex_);
    for (auto it = clients_.begin(); it != clients_.end();) {
        auto client = *it;
        if (authenticatedClients_.find(client) != authenticatedClients_.end() && authenticatedClients_[client]) {
            // Check if this client has access to the project
            std::string clientUsername = authenticatedUsernames_[client];
            bool clientHasAccess = false;

            for (const auto& [collab, role] : collaborators) {
                if (collab == clientUsername) {
                    clientHasAccess = true;
                    break;
                }
            }

            if (clientHasAccess) {
                boost::asio::async_write(*client, boost::asio::buffer(formattedMessage + "\n"),
                    [this, client](const boost::system::error_code& error, std::size_t) {
                        if (error) {
                            std::cerr << "Error sending message to client: " << error.message() << std::endl;
                            std::lock_guard<std::mutex> lock(clientMutex_);
                            authenticatedClients_.erase(client);
                            clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
                        }
                    });
            }
            ++it;
        }
        else {
            it = clients_.erase(it);
        }
    }
}
void NetworkServer::handleModelUpload(
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket,
    const std::string& content) {

    // Verify authentication
        {
            std::lock_guard<std::mutex> lock(clientMutex_);
            if (authenticatedClients_.find(socket) == authenticatedClients_.end() || !authenticatedClients_[socket]) {
                std::string errorMsg = "You must be authenticated to upload models.\n";
                boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                return;
            }
        }

        // Parse the content: projectId modelPath base64Data
        size_t firstSpace = content.find(' ');
        if (firstSpace == std::string::npos) {
            std::string errorMsg = "Invalid format for model upload.\n";
            boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
            return;
        }

        size_t secondSpace = content.find(' ', firstSpace + 1);
        if (secondSpace == std::string::npos) {
            std::string errorMsg = "Invalid format for model upload.\n";
            boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
            return;
        }

        std::string projectId = content.substr(0, firstSpace);
        std::string modelPath = content.substr(firstSpace + 1, secondSpace - firstSpace - 1);
        std::string base64Data = content.substr(secondSpace + 1);

        try {
            // Decode base64 data
            std::vector<char> modelData = base64_decode(base64Data);

            // Create server-side path
            std::string serverModelDir = "ServerModels/" + projectId;

            // Create all directories in the path
            std::error_code ec;
            if (!fs::create_directories(serverModelDir, ec) && ec) {
                std::string errorMsg = "Failed to create directory: " + ec.message() + "\n";
                boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                return;
            }

            std::string serverModelPath = serverModelDir + "/" + modelPath;

            // Create parent directories for the model file if needed
            size_t lastSlash = modelPath.find_last_of('/');
            if (lastSlash != std::string::npos) {
                std::string modelSubDir = serverModelDir + "/" + modelPath.substr(0, lastSlash);
                if (!fs::create_directories(modelSubDir, ec) && ec) {
                    std::string errorMsg = "Failed to create model subdirectory: " + ec.message() + "\n";
                    boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                    return;
                }
            }

            // Save the model file
            std::ofstream modelFile(serverModelPath, std::ios::binary);
            if (!modelFile) {
                std::string errorMsg = "Failed to open file for writing: " + serverModelPath + "\n";
                boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                return;
            }

            modelFile.write(modelData.data(), modelData.size());
            if (!modelFile) {
                std::string errorMsg = "Failed to write model data to file.\n";
                boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                return;
            }
            modelFile.close();

            // Store model path in database
            if (!dbManager_.storeModelForProject(projectId, modelPath)) {
                std::string errorMsg = "Failed to store model metadata in database.\n";
                boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                return;
            }

            std::string response = "MODEL_UPLOAD_OK " + projectId + " " + modelPath + "\n";
            boost::asio::async_write(*socket, boost::asio::buffer(response), [](auto, auto) {});

            // Notify other clients that a model has been updated
            notifyModelUpdated(socket, projectId, modelPath);
        }
        catch (const std::exception& e) {
            std::string errorMsg = "Exception during model upload: " + std::string(e.what()) + "\n";
            boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
        }
}

void NetworkServer::handleModelDownload(
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> socket,
    const std::string& content) {

    // Verify authentication
        {
            std::lock_guard<std::mutex> lock(clientMutex_);
            if (authenticatedClients_.find(socket) == authenticatedClients_.end() || !authenticatedClients_[socket]) {
                std::string errorMsg = "You must be authenticated to download models.\n";
                boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                return;
            }
        }

        // Parse the content: projectId modelPath
        size_t space = content.find(' ');
        if (space == std::string::npos) {
            std::string errorMsg = "Invalid format for model download.\n";
            boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
            return;
        }

        try {
            std::string projectId = content.substr(0, space);
            std::string modelPath = content.substr(space + 1);

            // Create server-side path
            std::string serverModelPath = "ServerModels/" + projectId + "/" + modelPath;

            // Check if file exists
            if (!fs::exists(serverModelPath)) {
                std::string response = "MODEL_NOT_FOUND " + projectId + " " + modelPath + "\n";
                boost::asio::async_write(*socket, boost::asio::buffer(response), [](auto, auto) {});
                return;
            }

            // Read the model file
            std::ifstream modelFile(serverModelPath, std::ios::binary);
            if (!modelFile) {
                std::string errorMsg = "Failed to open model file: " + serverModelPath + "\n";
                boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                return;
            }

            // Get file size
            modelFile.seekg(0, std::ios::end);
            size_t fileSize = modelFile.tellg();
            modelFile.seekg(0, std::ios::beg);

            // Read file content
            std::vector<char> buffer(fileSize);
            modelFile.read(buffer.data(), fileSize);
            if (!modelFile) {
                std::string errorMsg = "Failed to read model file data.\n";
                boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
                return;
            }
            modelFile.close();

            // Encode to base64
            std::string base64Data = base64_encode(reinterpret_cast<const unsigned char*>(buffer.data()), buffer.size());

            // Send response
            std::string response = "MODEL_DATA " + projectId + " " + modelPath + " " + base64Data + "\n";
            boost::asio::async_write(*socket, boost::asio::buffer(response), [](auto, auto) {});
        }
        catch (const std::exception& e) {
            std::string errorMsg = "Exception during model download: " + std::string(e.what()) + "\n";
            boost::asio::async_write(*socket, boost::asio::buffer(errorMsg), [](auto, auto) {});
        }
}

void NetworkServer::notifyModelUpdated(
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> sender,
    const std::string& projectId,
    const std::string& modelPath) {

    std::string notification = "MODEL_UPDATED " + projectId + " " + modelPath + "\n";

    std::lock_guard<std::mutex> lock(clientMutex_);
    for (auto& client : clients_) {
        if (client != sender && authenticatedClients_[client]) {
            boost::asio::async_write(*client, boost::asio::buffer(notification), [](auto, auto) {});
        }
    }
}
//1
void NetworkServer::BroadcastToCollaborators(const std::string& projectId, const std::string& message) {
    // Get all collaborators for this project
    auto collaborators = dbManager_.getCollaboratorsForProject(projectId);

    // Create a set of usernames who are collaborators
    std::unordered_set<std::string> collaboratorUsernames;
    for (const auto& [username, role] : collaborators) {
        collaboratorUsernames.insert(username);
    }

    // Lock the clients list
    std::lock_guard<std::mutex> lock(clientMutex_);

    // Send to all authenticated clients who are collaborators
    for (auto& client : clients_) {
        if (authenticatedClients_[client]) {
            std::string username = authenticatedUsernames_[client];
            if (collaboratorUsernames.find(username) != collaboratorUsernames.end()) {
                boost::asio::async_write(*client, boost::asio::buffer(message),
                    [this, client](const boost::system::error_code& error, std::size_t) {
                        if (error) {
                            std::cerr << "Error broadcasting to client: " << error.message() << std::endl;
                            std::lock_guard<std::mutex> lock(clientMutex_);
                            authenticatedClients_.erase(client);
                            clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
                        }
                    });
            }
        }
    }
}
//2
void NetworkServer::BroadcastToCollaborators(
    const std::string& projectId,
    const std::string& message,
    std::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> excludeClient) {

    // Get all collaborators for this project
    auto collaborators = dbManager_.getCollaboratorsForProject(projectId);

    // Create a set of usernames who are collaborators
    std::unordered_set<std::string> collaboratorUsernames;
    for (const auto& [username, role] : collaborators) {
        collaboratorUsernames.insert(username);
    }

    // Lock the clients list
    std::lock_guard<std::mutex> lock(clientMutex_);

    // Send to all authenticated clients who are collaborators except the excluded one
    for (auto& client : clients_) {
        if (client != excludeClient && authenticatedClients_[client]) {
            std::string username = authenticatedUsernames_[client];
            if (collaboratorUsernames.find(username) != collaboratorUsernames.end()) {
                boost::asio::async_write(*client, boost::asio::buffer(message),
                    [this, client](const boost::system::error_code& error, std::size_t) {
                        if (error) {
                            std::cerr << "Error broadcasting to client: " << error.message() << std::endl;
                            std::lock_guard<std::mutex> lock(clientMutex_);
                            authenticatedClients_.erase(client);
                            clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
                        }
                    });
            }
        }
    }
}
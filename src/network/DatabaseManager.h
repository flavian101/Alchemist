#pragma once

#include "SQLiteCpp/SQLiteCpp.h"
#include <string>
#include <vector>
#include <optional>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    // User management
    bool registerUser(const std::string& username, const std::string& password);
    bool createUser(const std::string& username, const std::string& password);
    bool authenticateUser(const std::string& username, const std::string& password);

    // Project management
    bool storeProject(const std::string& projectId, const std::string& name, const std::string& owner, const std::string& data);
    std::optional<std::string> loadProject(const std::string& projectId);
    std::vector<std::pair<std::string, std::string>> listProjectsForUser(const std::string& username);
    bool deleteProject(const std::string& projectId);

    // Project model management
    bool storeModelForProject(const std::string& projectId, const std::string& modelPath);
    std::optional<std::string> loadModelPath(const std::string& projectId);

    // Collaborator management
    bool addCollaborator(const std::string& projectId, const std::string& username, const std::string& role);
    bool removeCollaborator(const std::string& projectId, const std::string& username);
    std::vector<std::tuple<std::string, std::string>> getCollaboratorsForProject(const std::string& projectId);
    std::string getUserRole(const std::string& projectId, const std::string& username);

    // Access to database for raw queries (used by Server/Client Managers)
    SQLite::Database& getDatabase() { return db_; }

private:
    SQLite::Database db_;
};
#pragma once

#include "SQLiteCpp/SQLiteCpp.h"
#include <string>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();
    bool registerUser(const std::string& username, const std::string& password);
    bool createUser(const std::string& username, const std::string& password);
    bool authenticateUser(const std::string& username, const std::string& password);
    bool storeProject(const std::string& projectId, const std::string& name, const std::string& owner, const std::string& data);
    std::optional<std::string> loadProject(const std::string& projectId);
    std::vector<std::pair<std::string, std::string>> listProjectsForUser(const std::string& username);

private:
    SQLite::Database db_;
};


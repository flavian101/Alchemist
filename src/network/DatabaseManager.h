#pragma once

#include "SQLiteCpp/SQLiteCpp.h"
#include <string>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    bool createUser(const std::string& username, const std::string& password);
    bool authenticateUser(const std::string& username, const std::string& password);
    bool storeModel(const std::string& username, const std::string& modelData);

private:
    SQLite::Database db_;
};


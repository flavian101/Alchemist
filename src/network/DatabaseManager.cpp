#include "DatabaseManager.h"
#include <iostream>
#include <stdexcept>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : db_(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
    try {
        db_.exec("CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "username TEXT UNIQUE, "
            "password TEXT);");

        db_.exec("CREATE TABLE IF NOT EXISTS projects ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "project_id TEXT UNIQUE, "
            "name TEXT, "
            "owner TEXT, "
            "data TEXT, "
            "last_modified DATETIME DEFAULT CURRENT_TIMESTAMP);");

        db_.exec("CREATE TABLE IF NOT EXISTS project_users ("
            "project_id TEXT, "
            "username TEXT, "
            "role TEXT DEFAULT 'collaborator', "
            "PRIMARY KEY (project_id, username));");
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to create tables: " + std::string(e.what()));
    }
}

DatabaseManager::~DatabaseManager() {
    // Destructor
}

bool DatabaseManager::registerUser(const std::string& username, const std::string& password)
{
    try {
        SQLite::Statement query(db_, "INSERT INTO users (username, password) VALUES (?, ?)");
        query.bind(1, username);
        query.bind(2, password);
        query.exec();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Registration error: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::createUser(const std::string& username, const std::string& password) {
    try {
        SQLite::Statement query(db_, "INSERT INTO users (username, password) VALUES (?, ?);");
        query.bind(1, username);
        query.bind(2, password);
        query.exec();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to create user: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::authenticateUser(const std::string& username, const std::string& password) {
    try {
        SQLite::Statement query(db_, "SELECT COUNT(*) FROM users WHERE username = ? AND password = ?;");
        query.bind(1, username);
        query.bind(2, password);
        if (query.executeStep() && query.getColumn(0).getInt() > 0) {
            return true;
        }
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to authenticate user: " << e.what() << std::endl;
        return false;
    }
}
bool DatabaseManager::storeProject(const std::string& projectId, const std::string& name, const std::string& owner, const std::string& data) {
    try {
        SQLite::Statement check(db_, "SELECT COUNT(*) FROM projects WHERE project_id = ?");
        check.bind(1, projectId);
        bool exists = false;
        if (check.executeStep() && check.getColumn(0).getInt() > 0) {
            exists = true;
        }

        if (exists) {
            SQLite::Statement update(db_, "UPDATE projects SET name = ?, owner = ?, data = ?, last_modified = CURRENT_TIMESTAMP WHERE project_id = ?");
            update.bind(1, name);
            update.bind(2, owner);
            update.bind(3, data);
            update.bind(4, projectId);
            update.exec();
        }
        else {
            SQLite::Statement insert(db_, "INSERT INTO projects (project_id, name, owner, data) VALUES (?, ?, ?, ?)");
            insert.bind(1, projectId);
            insert.bind(2, name);
            insert.bind(3, owner);
            insert.bind(4, data);
            insert.exec();

            // Automatically assign access to the project owner
            SQLite::Statement access(db_, "INSERT INTO project_users (project_id, username, role) VALUES (?, ?, 'owner')");
            access.bind(1, projectId);
            access.bind(2, owner);
            access.exec();
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to store project: " << e.what() << std::endl;
        return false;
    }
}

std::optional<std::string> DatabaseManager::loadProject(const std::string& projectId) {
    try {
        SQLite::Statement query(db_, "SELECT data FROM projects WHERE project_id = ?");
        query.bind(1, projectId);

        if (query.executeStep()) {
            std::string data = query.getColumn(0).getText();
            return data;
        }
        else {
            return std::nullopt;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load project: " << e.what() << std::endl;
        return std::nullopt;
    }
}
std::vector<std::pair<std::string, std::string>> DatabaseManager::listProjectsForUser(const std::string& username) {
    std::vector<std::pair<std::string, std::string>> projects;
    try {
        SQLite::Statement query(db_,
            "SELECT p.project_id, p.name "
            "FROM projects p "
            "JOIN project_users u ON p.project_id = u.project_id "
            "WHERE u.username = ?");

        query.bind(1, username);

        while (query.executeStep()) {
            std::string id = query.getColumn(0).getText();
            std::string name = query.getColumn(1).getText();
            projects.emplace_back(id, name);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to list projects for user: " << e.what() << std::endl;
    }

    return projects;
}

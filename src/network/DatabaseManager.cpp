#include "DatabaseManager.h"
#include <iostream>
#include <stdexcept>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : db_(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
    try {
        db_.exec("PRAGMA foreign_keys = ON;");

        // Users table
        db_.exec("CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "username TEXT UNIQUE NOT NULL, "
            "password TEXT NOT NULL);");

        // Projects table
        db_.exec("CREATE TABLE IF NOT EXISTS projects ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "project_id TEXT UNIQUE NOT NULL, "
            "name TEXT NOT NULL, "
            "owner TEXT NOT NULL, "
            "data TEXT, "
            "last_modified DATETIME DEFAULT CURRENT_TIMESTAMP);");

        // Project Users table (to store collaborators)
        db_.exec("CREATE TABLE IF NOT EXISTS project_users ("
            "project_id TEXT, "
            "username TEXT, "
            "role TEXT DEFAULT 'collaborator', "
            "PRIMARY KEY (project_id, username), "
            "FOREIGN KEY (project_id) REFERENCES projects (project_id), "
            "FOREIGN KEY (username) REFERENCES users (username));");

        // Project Models table (to store models associated with projects)
        db_.exec("CREATE TABLE IF NOT EXISTS project_models ("
            "model_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "project_id TEXT NOT NULL, "
            "model_path TEXT NOT NULL, "
            "FOREIGN KEY (project_id) REFERENCES projects (project_id) ON DELETE CASCADE);");
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to create tables: " << e.what() << std::endl;
        throw std::runtime_error("Failed to create tables");
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
    // Same as registerUser but with different function name for clarity
    return registerUser(username, password);
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
            addCollaborator(projectId, owner, "owner");
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

bool DatabaseManager::deleteProject(const std::string& projectId) {
    try {
        // First delete collaborators
        SQLite::Statement deleteCollaborators(db_, "DELETE FROM project_users WHERE project_id = ?");
        deleteCollaborators.bind(1, projectId);
        deleteCollaborators.exec();

        // Delete models
        SQLite::Statement deleteModels(db_, "DELETE FROM project_models WHERE project_id = ?");
        deleteModels.bind(1, projectId);
        deleteModels.exec();

        // Delete project
        SQLite::Statement deleteProject(db_, "DELETE FROM projects WHERE project_id = ?");
        deleteProject.bind(1, projectId);
        deleteProject.exec();

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to delete project: " << e.what() << std::endl;
        return false;
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

bool DatabaseManager::storeModelForProject(const std::string& projectId, const std::string& modelPath) {
    try {
        SQLite::Statement query(db_, "INSERT INTO project_models (project_id, model_path) VALUES (?, ?)");
        query.bind(1, projectId);
        query.bind(2, modelPath);
        query.exec();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to store model for project: " << e.what() << std::endl;
        return false;
    }
}

std::optional<std::string> DatabaseManager::loadModelPath(const std::string& projectId) {
    try {
        SQLite::Statement query(db_, "SELECT model_path FROM project_models WHERE project_id = ?");
        query.bind(1, projectId);

        if (query.executeStep()) {
            return query.getColumn(0).getText();
        }
        else {
            return std::nullopt;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load model path: " << e.what() << std::endl;
        return std::nullopt;
    }
}

// Collaborator management functions
bool DatabaseManager::addCollaborator(const std::string& projectId, const std::string& username, const std::string& role) {
    try {
        // First check if user exists
        SQLite::Statement userCheck(db_, "SELECT COUNT(*) FROM users WHERE username = ?");
        userCheck.bind(1, username);

        if (userCheck.executeStep() && userCheck.getColumn(0).getInt() > 0) {
            // User exists, add collaborator
            SQLite::Statement query(db_, "INSERT OR REPLACE INTO project_users (project_id, username, role) VALUES (?, ?, ?)");
            query.bind(1, projectId);
            query.bind(2, username);
            query.bind(3, role);
            query.exec();
            return true;
        }
        else {
            std::cerr << "User does not exist: " << username << std::endl;
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to add collaborator: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::removeCollaborator(const std::string& projectId, const std::string& username) {
    try {
        // Don't allow removing the owner
        if (getUserRole(projectId, username) == "owner") {
            std::cerr << "Cannot remove the owner of a project" << std::endl;
            return false;
        }

        SQLite::Statement query(db_, "DELETE FROM project_users WHERE project_id = ? AND username = ?");
        query.bind(1, projectId);
        query.bind(2, username);
        query.exec();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to remove collaborator: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::tuple<std::string, std::string>> DatabaseManager::getCollaboratorsForProject(const std::string& projectId) {
    std::vector<std::tuple<std::string, std::string>> collaborators;
    try {
        SQLite::Statement query(db_, "SELECT username, role FROM project_users WHERE project_id = ?");
        query.bind(1, projectId);

        while (query.executeStep()) {
            std::string username = query.getColumn(0).getText();
            std::string role = query.getColumn(1).getText();
            collaborators.emplace_back(username, role);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to get collaborators: " << e.what() << std::endl;
    }

    return collaborators;
}

std::string DatabaseManager::getUserRole(const std::string& projectId, const std::string& username) {
    try {
        SQLite::Statement query(db_, "SELECT role FROM project_users WHERE project_id = ? AND username = ?");
        query.bind(1, projectId);
        query.bind(2, username);

        if (query.executeStep()) {
            return query.getColumn(0).getText();
        }
        else {
            return "none";  // User has no role in this project
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to get user role: " << e.what() << std::endl;
        return "none";
    }
}


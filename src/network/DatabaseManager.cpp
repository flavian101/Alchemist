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

        db_.exec("CREATE TABLE IF NOT EXISTS models ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "username TEXT, "
            "modelData TEXT);");
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

bool DatabaseManager::storeModel(const std::string& username, const std::string& modelData) {
    try {
        SQLite::Statement query(db_, "INSERT INTO models (username, modelData) VALUES (?, ?);");
        query.bind(1, username);
        query.bind(2, modelData);
        query.exec();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to store model: " << e.what() << std::endl;
        return false;
    }
}

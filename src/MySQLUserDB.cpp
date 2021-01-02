#include "MySQLUserDB.h"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <exception>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <sstream>
#include <string>

std::unique_ptr<sql::Connection> openDB(
    const std::string &host,
    const std::string &user,
    const std::string &password) {
    sql::mysql::MySQL_Driver *driver;
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        return std::unique_ptr<sql::Connection>(
            driver->connect(host, user, password));
    } catch (sql::SQLException &e) {
        std::string message = "サーバーとの接続に失敗しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}

void MySQLUserDB::addInternalDatabase(
    const std::string name,
    const std::string hashedPass,
    const bool avail,
    const t_Level level) {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto pStmt = std::unique_ptr<sql::PreparedStatement>(
            connect->prepareStatement(
                "INSERT INTO user(name,password,avail,level) VALUES(?,?,?,?)"));
        pStmt->setString(1, name);
        pStmt->setString(2, hashedPass);
        pStmt->setBoolean(3, avail);
        pStmt->setUInt(4, level);
        pStmt->execute();
    } catch (const sql::SQLException &e) {
        std::string message = "Mysqlにエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}

User MySQLUserDB::searchByIdInternalDatabase(const uint id) {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto pStmt = std::unique_ptr<sql::PreparedStatement>(connect->prepareStatement("SELECT * from user WHERE id= ? LIMIT 1"));
        pStmt->setUInt(1, id);
        auto result = std::unique_ptr<sql::ResultSet>(pStmt->executeQuery());
        if (result->next()) {
            return User(
                result->getUInt(1),
                result->getString(2).asStdString(),
                result->getString(3).asStdString(),
                result->getBoolean(4),
                intToT_Level(result->getUInt(5)));
        } else {
            throw std::range_error("アカウントが見つかりませんでした");
        }
    } catch (const sql::SQLException &e) {
        std::string message = "Mysqlにエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}

std::vector<User> MySQLUserDB::searchByNameInternalDatabase(const std::string name) {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    std::vector<User> userData;
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto pStmt = std::unique_ptr<sql::PreparedStatement>(connect->prepareStatement("SELECT * from user WHERE name LIKE ?"));
        pStmt->setString(1, "%" + name + "%");
        auto result = std::unique_ptr<sql::ResultSet>(pStmt->executeQuery());
        while (result->next()) {
            userData.push_back(User(
                result->getUInt(1),
                result->getString(2).asStdString(),
                result->getString(3).asStdString(),
                result->getBoolean(4),
                intToT_Level(result->getUInt(5))));
        }
        if (userData.empty()) {
            throw std::range_error("アカウントが見つかりませんでした");
        }
        return userData;
    } catch (const sql::SQLException &e) {
        std::string message = "Mysqlにエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}

std::vector<User> MySQLUserDB::searchByAvailInternalDatabase(const bool avail) {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    std::vector<User> userData;
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto pStmt = std::unique_ptr<sql::PreparedStatement>(connect->prepareStatement("SELECT * from user WHERE avail=?"));
        pStmt->setBoolean(1, avail);
        auto result = std::unique_ptr<sql::ResultSet>(pStmt->executeQuery());
        while (result->next()) {
            userData.push_back(User(
                result->getUInt(1),
                result->getString(2).asStdString(),
                result->getString(3).asStdString(),
                result->getBoolean(4),
                intToT_Level(result->getUInt(5))));
        }
        if (userData.empty()) {
            throw std::range_error("アカウントが見つかりませんでした");
        }
        return userData;
    } catch (const sql::SQLException &e) {
        std::string message = "Mysqlにエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}

std::vector<User> MySQLUserDB::searchByLevelInternalDatabase(const t_Level level) {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    std::vector<User> userData;
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto pStmt = std::unique_ptr<sql::PreparedStatement>(connect->prepareStatement("SELECT * from user WHERE level=?"));
        pStmt->setUInt(1, level);
        auto result = std::unique_ptr<sql::ResultSet>(pStmt->executeQuery());
        while (result->next()) {
            userData.push_back(User(
                result->getUInt(1),
                result->getString(2).asStdString(),
                result->getString(3).asStdString(),
                result->getBoolean(4),
                intToT_Level(result->getUInt(5))));
        }
        if (userData.empty()) {
            throw std::range_error("アカウントが見つかりませんでした");
        }
        return userData;
    } catch (const sql::SQLException &e) {
        std::string message = "Mysqlにエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}

void MySQLUserDB::removeInternalDatabase(const uint id) {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto pStmt = std::unique_ptr<sql::PreparedStatement>(connect->prepareStatement("DELETE FROM user WHERE id = ? LIMIT 1"));
        pStmt->setUInt(1, id);
        pStmt->execute();
    } catch (const sql::SQLException &e) {
        std::string message = "Mysqlにエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}

void MySQLUserDB::updateInternalDatabase(
    const uint updateUserId,
    const std::string name,
    const std::string hashedPass,
    const bool avail,
    const t_Level level) {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    // アカウントがなければ、例外を返却
    std::string pass;
    try {
        auto user = searchById(updateUserId);
        // hashedPassが空文字でなければ、パスワードを変更
        if (hashedPass.empty()) {
            pass = user.Pass;
        } else {
            pass = hashedPass;
        }
    } catch (const std::range_error &e) {
        throw;
    }
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto pStmt = std::unique_ptr<sql::PreparedStatement>(
            connect->prepareStatement("UPDATE user SET name = ? , password = ? , avail = ? , level = ? WHERE id = ? LIMIT 1"));
        pStmt->setString(1, name);
        pStmt->setString(2, pass);
        pStmt->setBoolean(3, avail);
        pStmt->setUInt(4, level);
        pStmt->setUInt(5, updateUserId);
        pStmt->execute();
    } catch (const sql::SQLException &e) {
        std::string message = "Mysqlにエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}

std::vector<User> MySQLUserDB::GetAllUserData() {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    std::vector<User> userData;
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto result = std::unique_ptr<sql::ResultSet>(stmt->executeQuery("SELECT * from user"));
        while (result->next()) {
            userData.push_back(User(
                result->getUInt(1),
                result->getString(2).asStdString(),
                result->getString(3).asStdString(),
                result->getBoolean(4),
                intToT_Level(result->getUInt(5))));
        }
    } catch (const sql::SQLException &e) {
        std::string message = "Mysqlにエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
    return userData;
}
// ok
uint MySQLUserDB::GetLastId() {
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        auto result = std::unique_ptr<sql::ResultSet>(stmt->executeQuery("SELECT MAX(id) from user"));
        result->next();
        return result->getUInt(1);
    } catch (sql::SQLException &e) {
        std::string message = "MySqlでエラーが発生しました:";
        message.append(e.what());
        throw std::runtime_error(message);
    }
}
// ok
MySQLUserDB::MySQLUserDB(
    const std::string host,
    const std::string user,
    const std::string password,
    const std::string database) {
    mysqlHost = host;
    mysqlUser = user;
    mysqlPassword = password;
    mysqlDatabase = database;
    std::unique_ptr<sql::Connection> connect;
    try {
        connect = openDB(mysqlHost, mysqlUser, mysqlPassword);
    } catch (const std::runtime_error &e) {
        throw;
    }
    try {
        auto stmt = std::unique_ptr<sql::Statement>(connect->createStatement());
        stmt->execute("USE " + mysqlDatabase);
        stmt->execute(
            "CREATE TABLE IF NOT EXISTS user("
            "id INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY,"
            "name VARCHAR(32) NOT NULL,"
            "password VARCHAR(255) NOT NULL,"
            "avail BOOLEAN,"
            "level TINYINT UNSIGNED NOT NULL"
            ") Engine=InnoDB DEFAULT CHARSET=utf8mb4");
    } catch (sql::SQLException &e) {
        std::string message = "MySqlでエラーが発生しました:";
        message.append(e.what());
    }
}

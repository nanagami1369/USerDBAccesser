#include "User.h"
#include "UserDB.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include "SQLiteUserDB.h"

void SQLiteUserDB::addInternalDatabase(
    const std::string name,
    const std::string hashedPass,
    const bool avail,
    const t_Level level) {
    // indexは、SQliteから自動で割り当てられるので一旦0を入れる。
    auto index = "0";
    auto user = new User(index, name, hashedPass, avail, level);
    try {
        std::stringstream query;
        query << "INSERT INTO user VALUES (NULL, '"
              << user->Name << "', '"
              << user->Pass << "', "
              << (int)user->Avail << ", "
              << (int)user->Level << ")";
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        db.exec(query.str());
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

User SQLiteUserDB::searchInternalDatabase(const std::string id) {
    auto idNumber = std::stoi(id);
    try {
        std::stringstream queryString;
        queryString << "SELECT * FROM user WHERE id=" << idNumber << " LIMIT 1";
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, queryString.str());
        if (query.executeStep()) {
            auto id = query.getColumn(0);
            auto name = query.getColumn(1);
            auto pass = query.getColumn(2);
            auto avail = query.getColumn(3).getInt() == 1 ? true : false;
            auto level = intToT_Level(query.getColumn(4));
            return User(id, name, pass, avail, level);
        } else {
            throw std::range_error("アカウントが見つかりませんでした");
        }
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}
void SQLiteUserDB::removeInternalDatabase(const std::string id) {
    auto idNumber = std::stoi(id);
    try {
        std::stringstream query;
        query << "DELETE FROM user WHERE id=" << idNumber;
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        std::cout << query.str() << std::endl;
        db.exec(query.str());
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

void SQLiteUserDB::updateInternalDatabase(
    const std::string updateUserId,
    const std::string name,
    const std::string hashedPass,
    const bool avail,
    const t_Level level) {
    std::string pass;

    // アカウントがなければ、例外を返却
    try {
        auto user = search(updateUserId);
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

        std::stringstream query;
        query << "UPDATE user SET"
              << " name = '" << name << "',"
              << " pass = '" << pass << "',"
              << " avail = " << (int)avail << ","
              << " level = " << (int)level
              << " WHERE id = " << updateUserId;
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        db.exec(query.str());
    } catch (const std::range_error &e) {
        throw;
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

void SQLiteUserDB::WriterAllUserToConsole() {
    try {
        SQLite::Database db(dbName, SQLite::OPEN_READONLY);
        SQLite::Statement query(db, "SELECT * FROM user");
        std::cout << " ID | Name | Pass | avail | Level" << std::endl;
        while (query.executeStep()) {
            auto id = query.getColumn(0);
            auto name = query.getColumn(1);
            auto pass = query.getColumn(2);
            auto avail = query.getColumn(3).getInt() == 1 ? true : false;
            auto level = intToT_Level(query.getColumn(4));
            auto user = new User(id, name, pass, avail, level);
            std::cout << user->toString() << std::endl;
        }
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}
SQLiteUserDB::SQLiteUserDB() {
    try {
        SQLite::Database db("user.sqlite3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db.exec("CREATE TABLE IF NOT EXISTS user (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, pass TEXT, avail NUMERIC, level INTGER)");
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

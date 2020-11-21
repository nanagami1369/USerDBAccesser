#include "SQLiteUserDB.h"
#include "User.h"
#include "UserDB.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

void SQLiteUserDB::addInternalDatabase(
    const std::string name,
    const std::string hashedPass,
    const bool avail,
    const t_Level level) {
    // indexは、SQliteから自動で割り当てられるので一旦0を入れる。
    const uint index = 0;
    auto user = new User(index, name, hashedPass, avail, level);
    try {
        const char *baseQuery = "INSERT INTO user VALUES ( NULL, ?, ?, ?, ?)";
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, baseQuery);
        SQLite::bind(query, user->Name, user->Pass, (int)user->Avail, (int)user->Level);
        query.exec();
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

User SQLiteUserDB::searchByIdInternalDatabase(const uint id) {
    try {
        const char *baseQuery = "SELECT * FROM user WHERE id= ? LIMIT 1";
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, baseQuery);
        SQLite::bind(query, id);
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

std::vector<User> SQLiteUserDB::searchByNameInternalDatabase(const std::string name) {
    try {
        auto searchWord = "%" + name + "%";
        const char *baseQuery = "SELECT * FROM user WHERE name LIKE ? ";
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, baseQuery);
        SQLite::bind(query, searchWord);
        std::vector<User> searchedUsers;
        while (query.executeStep()) {
            auto id = query.getColumn(0);
            auto name = query.getColumn(1);
            auto pass = query.getColumn(2);
            auto avail = query.getColumn(3).getInt() == 1 ? true : false;
            auto level = intToT_Level(query.getColumn(4));
            searchedUsers.push_back(User(id, name, pass, avail, level));
        }
        if (searchedUsers.empty()) {
            throw std::range_error("アカウントが見つかりませんでした");
        }
        return searchedUsers;
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

std::vector<User> SQLiteUserDB::searchByAvailInternalDatabase(const bool avail) {
    try {
        const char *baseQuery = "SELECT * FROM user WHERE avail = ? ";
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, baseQuery);
        SQLite::bind(query, (int)avail);
        std::vector<User> searchedUsers;
        while (query.executeStep()) {
            auto id = query.getColumn(0);
            auto name = query.getColumn(1);
            auto pass = query.getColumn(2);
            auto avail = query.getColumn(3).getInt() == 1 ? true : false;
            auto level = intToT_Level(query.getColumn(4));
            searchedUsers.push_back(User(id, name, pass, avail, level));
        }
        if (searchedUsers.empty()) {
            throw std::range_error("アカウントが見つかりませんでした");
        }
        return searchedUsers;
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

std::vector<User> SQLiteUserDB::searchByLevelInternalDatabase(const t_Level level) {
    const char *baseQuery = "SELECT * FROM user WHERE level = ? ";
    SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
    SQLite::Statement query(db, baseQuery);
    SQLite::bind(query, (int)level);
    std::vector<User> searchedUsers;
    while (query.executeStep()) {
        auto id = query.getColumn(0);
        auto name = query.getColumn(1);
        auto pass = query.getColumn(2);
        auto avail = query.getColumn(3).getInt() == 1 ? true : false;
        auto level = intToT_Level(query.getColumn(4));
        searchedUsers.push_back(User(id, name, pass, avail, level));
    }
    if (searchedUsers.empty()) {
        throw std::range_error("アカウントが見つかりませんでした");
    }
    return searchedUsers;
}

void SQLiteUserDB::removeInternalDatabase(const uint id) {
    try {
        const char *baseQuery = "DELETE FROM user WHERE id = ? ";
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, baseQuery);
        SQLite::bind(query, id);
        query.exec();
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

void SQLiteUserDB::updateInternalDatabase(
    const uint updateUserId,
    const std::string name,
    const std::string hashedPass,
    const bool avail,
    const t_Level level) {
    std::string pass;

    // アカウントがなければ、例外を返却
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
        const char *baseQuery = "UPDATE user SET name = ? , pass = ? , avail = ? , level = ? WHERE id = ?";
        SQLite::Database db(dbName, SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, baseQuery);
        SQLite::bind(query, name, pass, (int)avail, (int)level, updateUserId);
        query.exec();
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
        std::cout << " ID | Name | Pass | avail | Level" << '\n';
        while (query.executeStep()) {
            auto id = query.getColumn(0);
            auto name = query.getColumn(1);
            auto pass = query.getColumn(2);
            auto avail = query.getColumn(3).getInt() == 1 ? true : false;
            auto level = intToT_Level(query.getColumn(4));
            auto user = new User(id, name, pass, avail, level);
            std::cout << user->toString() << '\n';
        }
    } catch (const SQLite::Exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}
uint SQLiteUserDB::GetLastId() {
    try {
        SQLite::Database db(dbName, SQLite::OPEN_READONLY);
        SQLite::Statement query(db, "SELECT id FROM user ORDER BY id DESC LIMIT 1");
        if (query.executeStep()) {
            return query.getColumn(0);
        } else {
            throw std::range_error("アカウントが存在しません");
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

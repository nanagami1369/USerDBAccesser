#include "User.h"
#include "UserDB.h"
#include "getHashPassWord.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <exception>
#include <iostream>
#include <regex>
#include <sstream>

static const char *pattern = "^\\$[56]\\$";
static const std::regex re(pattern);

void UserDB::add(
    std::string name,
    std::string pass,
    bool avail,
    t_Level level) {
    if (name.empty() || name.length() == 0) {
        throw std::invalid_argument("Nameが空文字かNullです");
    }
    if (pass.empty() || pass.length() == 0) {
        throw std::invalid_argument("Passが空文字かNullです");
    }
    if (std::regex_search(pass, re)) {
        // ハッシュ化されたパスワードかどうかの判定に、
        //「$5$」、「$6$」が先頭についてるかで判別するために禁止
        throw std::invalid_argument("パスワードの先頭には、「$5$」、「$6$」を使えません");
    }
    auto user = new User(std::to_string(index), name, getHashPassWord(pass), avail, level);
    try {
        std::stringstream query;
        query << "INSERT INTO user VALUES (NULL, '"
              << user->Name << "', '"
              << user->Pass << "', "
              << (int)user->Avail << ", "
              << (int)user->Level << ")";
        SQLite::Database db("user.sqlite3", SQLite::OPEN_READWRITE);
        db.exec(query.str());
    } catch (const std::exception &e) {
        std::cerr << "SQLiteでエラーが発生しました。: " << e.what() << std::endl;
    }
}

User UserDB::search(std::string id) {
    auto idNumber = 0;
    try {
        idNumber = std::stoi(id);
    } catch (const std::invalid_argument) {
        throw std::invalid_argument("IDは数字のみです");

    } catch (const std::out_of_range) {
        throw std::out_of_range("値が大きすぎるか小さすぎます");
    }
    try {
        std::stringstream queryString;
        queryString << "SELECT * FROM user WHERE id=" << idNumber << " LIMIT 1";
        SQLite::Database db("user.sqlite3", SQLite::OPEN_READWRITE);
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
    } catch (const std::exception &e) {
        throw;
    }
}

void UserDB::remove(std::string id) {
    auto idNumber = 0;
    try {
        idNumber = std::stoi(id);
    } catch (const std::invalid_argument) {
        throw std::invalid_argument("IDは数字のみです");

    } catch (const std::out_of_range) {
        throw std::out_of_range("値が大きすぎるか小さすぎます");
    }
    try {
        std::stringstream query;
        query << "DELETE FROM user WHERE id=" << idNumber;
        SQLite::Database db("user.sqlite3", SQLite::OPEN_READWRITE);
        std::cout << query.str() << std::endl;
        db.exec(query.str());
    } catch (const std::exception &e) {
        std::cerr << "SQLiteでエラーが発生しました。: " << e.what() << std::endl;
    }
}

void UserDB::update(
    std::string updateUserId,
    std::string name,
    std::string pass,
    bool avail,
    t_Level level) {
    if (name.empty() || name.length() == 0) {
        throw std::invalid_argument("Nameが空文字かNullです");
    }
    if (pass.empty() || pass.length() == 0) {
        throw std::invalid_argument("Passが空文字かNullです");
    }

    // アカウントの存在確認用
    // アカウントがなければ、例外を返却
    try {
        search(updateUserId);
    } catch (const std::range_error &e) {
        throw;
    }

    try {
        std::string hashPassword = pass;
        auto isHashPassWord = std::regex_search(pass, re);
        if (!isHashPassWord) {
            hashPassword = getHashPassWord(pass);
        }

        std::stringstream query;
        query << "UPDATE user SET"
              << " name = '" << name << "',"
              << " pass = '" << hashPassword << "',"
              << " avail = " << (int)avail << ","
              << " level = " << (int)level
              << " WHERE id = " << updateUserId;
        std::cout << query.str() << '\n';
        SQLite::Database db("user.sqlite3", SQLite::OPEN_READWRITE);
        db.exec(query.str());
    } catch (const std::range_error &e) {
        throw;
    } catch (const std::exception &e) {
        std::stringstream message;
        message << "SQLiteでエラーが発生しました。: " << e.what();
        throw std::runtime_error(message.str());
    }
}

void UserDB::WriterAllUserToConsole() {
    try {
        SQLite::Database db("user.sqlite3", SQLite::OPEN_READONLY);
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
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
}

UserDB::UserDB() {
    try {
        SQLite::Database db("user.sqlite3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        db.exec("CREATE TABLE IF NOT EXISTS user (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, pass TEXT, avail NUMERIC, level INTGER)");

    } catch (const std::exception &e) {
        std::cerr << "SQLiteでエラーが発生しました。: " << e.what() << std::endl;
    }
}
UserDB::~UserDB() {}

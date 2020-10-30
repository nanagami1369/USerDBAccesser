#include "User.h"
#include "UserDB.h"
#include "getHashPassWord.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <exception>
#include <iostream>
#include <sstream>

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

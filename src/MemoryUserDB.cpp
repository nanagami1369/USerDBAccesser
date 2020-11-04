#include "User.h"
#include "UserDB.h"
#include "getHashPassWord.h"
#include <exception>
#include <iostream>

const int searchIndex(std::string id, const std::vector<std::shared_ptr<User>> memory) {
    int min = 0;
    int max = memory.size() - 1;
    int mid;
    while (min <= max) {
        mid = (min + max) / 2;
        if (memory[mid].get()->ID == id) {
            return mid;
        }
        if (memory[mid].get()->ID < id) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }
    throw std::range_error("アカウントが見つかりませんでした");
}

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
    index++;
    std::shared_ptr<User> userShearPtr(user);
    Memory.push_back(userShearPtr);
}

User UserDB::search(std::string id) {
    int min = 0;
    int max = Memory.size() - 1;
    int mid;
    while (min <= max) {
        mid = (min + max) / 2;
        if (Memory[mid].get()->ID == id) {
            auto user = Memory[mid].get();
            return User(
                user->ID,
                user->Name,
                user->Pass,
                user->Avail,
                user->Level);
        }
        if (Memory[mid].get()->ID < id) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }
    throw std::range_error("アカウントが見つかりませんでした");
}

void UserDB::remove(std::string id) {
    try {
        auto index = searchIndex(id, Memory);

        Memory.erase(Memory.begin() + index);
    } catch (const std::range_error &e) {
        throw;
    }
}

void UserDB::WriterAllUserToConsole() {
    std::cout << " ID | Name | Pass | avail | Level" << std::endl;
    for (auto &user : this->Memory) {
        std::cout << user->toString() << std::endl;
    }
}

UserDB::UserDB() {
    this->add("田中", "1243251", true, ADMIN);
    this->add("中田", "24332243", true, TRY);
}
UserDB::~UserDB() {}

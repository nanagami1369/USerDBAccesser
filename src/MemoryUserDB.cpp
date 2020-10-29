#include "User.h"
#include "UserDB.h"
#include <exception>
#include <iostream>
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
    auto user = new User(std::to_string(index), name, pass, avail, level);
    index++;
    std::shared_ptr<User> userShearPtr(user);
    Memory.push_back(userShearPtr);
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

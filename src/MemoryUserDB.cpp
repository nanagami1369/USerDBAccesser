#include "User.h"
#include "UserDB.h"
#include <crypt.h>
#include <exception>
#include <iostream>

std::string getHashPassWord(std::string pass) {

    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        throw std::runtime_error("パスワードのハッシュ化に失敗しました。");
    }
    srandom(ts.tv_nsec ^ ts.tv_sec);
    auto r = random();
    std::string salt = "$5$" + std::to_string(r);
    struct crypt_data data;
    data.initialized = 0;
    if (crypt_r(pass.c_str(), salt.c_str(), &data) == NULL) {
        throw std::runtime_error("パスワードのハッシュ化に失敗しました。");
    };
    return std::string(data.keysched);
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

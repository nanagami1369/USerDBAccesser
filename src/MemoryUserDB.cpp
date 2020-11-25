#include "MemoryUserDB.h"
#include "User.h"
#include "UserDB.h"
#include <regex>

int MemoryUserDB::searchIndex(const uint id) {
    int min = 0;
    int max = Memory.size() - 1;
    int mid;
    while (min <= max) {
        mid = (min + max) / 2;
        if (Memory[mid].get()->ID == id) {
            return mid;
        }
        if (Memory[mid].get()->ID < id) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }
    throw std::range_error("アカウントが見つかりませんでした");
}

void MemoryUserDB::addInternalDatabase(
    const std::string name,
    const std::string hashedPass,
    const bool avail,
    const t_Level level) {
    auto user = new User(index, name, hashedPass, avail, level);
    index++;
    std::shared_ptr<User> userShearPtr(user);
    Memory.push_back(userShearPtr);
}
User MemoryUserDB::searchByIdInternalDatabase(const uint id) {
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
std::vector<User> MemoryUserDB::searchByNameInternalDatabase(const std::string name) {
    auto pattern = std::regex("^(?=.*" + name + ").*$");
    std::vector<User> searchedUsers;
    for (auto user : Memory) {
        if (std::regex_match(user->Name, pattern)) {
            searchedUsers.push_back(User(
                user->ID,
                user->Name,
                user->Pass,
                user->Avail,
                user->Level));
        }
    }
    if (searchedUsers.empty()) {
        throw std::range_error("アカウントが見つかりませんでした");
    }

    return searchedUsers;
}

std::vector<User> MemoryUserDB::searchByAvailInternalDatabase(const bool avail) {
    std::vector<User> searchedUsers;
    for (auto user : Memory) {
        if (user->Avail == avail) {
            searchedUsers.push_back(User(
                user->ID,
                user->Name,
                user->Pass,
                user->Avail,
                user->Level));
        }
    }
    if (searchedUsers.empty()) {
        throw std::range_error("アカウントが見つかりませんでした");
    }
    return searchedUsers;
}

std::vector<User> MemoryUserDB::searchByLevelInternalDatabase(const t_Level level) {
    std::vector<User> searchedUsers;
    for (auto user : Memory) {
        if (user->Level == level) {
            searchedUsers.push_back(User(
                user->ID,
                user->Name,
                user->Pass,
                user->Avail,
                user->Level));
        }
    }
    if (searchedUsers.empty()) {
        throw std::range_error("アカウントが見つかりませんでした");
    }
    return searchedUsers;
}

void MemoryUserDB::removeInternalDatabase(const uint id) {
    try {
        auto index = searchIndex(id);

        Memory.erase(Memory.begin() + index);
    } catch (const std::range_error &e) {
        throw;
    }
}

void MemoryUserDB::updateInternalDatabase(
    const uint updateUserId,
    const std::string name,
    const std::string hashedPass,
    const bool avail,
    const t_Level level) {
    int index;
    try {
        index = searchIndex(updateUserId);
    } catch (const std::range_error &e) {
        throw;
    }
    auto user = Memory[index].get();
    user->Name = name;
    // hashedPassが空文字でなければ、パスワードを変更
    if (!(hashedPass.empty())) {
        user->Pass = hashedPass;
    }
    user->Avail = avail;
    user->Level = level;
}

std::vector<User> MemoryUserDB::GetAllUserData() {
    std::vector<User> userData;
    for (auto &user : this->Memory) {
        userData.push_back(User(
            user->ID,
            user->Name,
            user->Pass,
            user->Avail,
            user->Level));
    }
    return userData;
}

uint MemoryUserDB::GetLastId() {
    if (Memory.size() == 0) {
        throw std::range_error("アカウントが存在しません");
    }

    return Memory.back()->ID;
}

MemoryUserDB::MemoryUserDB() {
    this->add("テスト 田中", "12345678", true, t_Level::ADMIN);
    this->add("テスト 中田", "12345678", false, t_Level::GEN);
    this->add("テスト 明美", "12345678", true, t_Level::TRY);
    this->add("テスト 未央", "12345678", true, t_Level::PREM);
    this->add("テスト 太郎", "12345678", true, t_Level::ADMIN);
    this->add("テスト 中島", "12345678", true, t_Level::GEN);
    this->add("テスト 早川", "12345678", true, t_Level::TRY);
    this->add("テスト マイケル", "12345678", true, t_Level::GEN);
    this->add("テスト 長谷川", "12345678", false, t_Level::ADMIN);
    this->add("テスト 岡本", "12345678", true, t_Level::PREM);
    this->add("テスト 翔", "12345678", true, t_Level::PREM);
    this->add("テスト 健", "12345678", true, t_Level::TRY);
}

#pragma once
#include "User.h"
#include "getHashPassWord.h"
#include "t_level.h"
#include <memory>
#include <string>
#include <vector>

class UserDB {
  private:
    std::vector<std::shared_ptr<User>> Memory;
    uint index = 0;

  public:
    void add(
        std::string name,
        std::string pass,
        bool avail,
        t_Level level);
    User search(std::string id);
    void remove(std::string id);
    void WriterAllUserToConsole();
    UserDB();
    ~UserDB();
};

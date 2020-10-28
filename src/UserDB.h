#pragma once
#include "User.h"
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
    void WriterAllUserToConsole();
};

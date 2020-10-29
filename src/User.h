#pragma once
#include <string>
#include "t_level.h"
class User {
  public:
    std::string ID;
    std::string Name;
    std::string Pass;
    bool Avail;
    t_Level Level;
    User(std::string id,
         std::string name,
         std::string pass,
         bool avail,
         t_Level level);
    std::string toString();
};

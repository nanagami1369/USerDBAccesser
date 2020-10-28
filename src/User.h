#pragma once
#include <string>
enum t_Level {
    ADMIN,
    PREM,
    GEN,
    TRY
};

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

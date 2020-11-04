#pragma once
#include <string>
#include "UserDB.h"
class Controller {
  private:
    UserDB *db;
    std::string readName();
    std::string readPassward();
    bool readIsAvail();
    t_Level readLevel();
    void removeUser();
  public:
    Controller(UserDB *db);
    ~Controller();
    void start();
};

#pragma once
#include "UserDB.h"
#include <string>
class Controller {
  private:
    UserDB *db;
    std::string readName();
    std::string readPassward();
    bool readIsAvail();
    t_Level readLevel();
    void removeUser();
    void changeAvail();
  public:
    Controller(UserDB *db);
    ~Controller();
    void start();
};

#pragma once
#include "UserDB.h"
#include <string>
class Controller {
  private:
    UserDB *db;

    uint InputIdPrompt();

    void addUser();
    void removeUser();
    void changeAvail();

  public:
    Controller(UserDB *db);
    ~Controller();
    void start();
};

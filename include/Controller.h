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
    void searchUser();
  public:
    Controller(UserDB *db);
    ~Controller();
    void start();
};

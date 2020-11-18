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

    void searchUserById();
    void searchUserByName();
    void searchUserByAvail();
    void searchUserByLevel();

  public:
    Controller(UserDB *db);
    ~Controller();
    void start();
};

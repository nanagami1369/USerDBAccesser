#include "Controller.h"
#include "User.h"
#include "UserDB.h"
int main(int argc, char const *argv[]) {
    auto db = new UserDB();
    auto controller = new Controller(db);
    controller->start();
    delete controller;
    return 0;
}

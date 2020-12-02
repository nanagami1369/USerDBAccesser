#ifdef MEMORY_DB
#include "MemoryUserDB.h"
#elif SQLITE3_DB
#include "SQLiteUserDB.h"
#endif
#include "Controller.h"
#include "User.h"
#include "UserDB.h"
int main(int argc, char const *argv[]) {
    UserDB *db;
#ifdef MEMORY_DB
    db = new MemoryUserDB();
#elif SQLITE3_DB
    db = new SQLiteUserDB("user.sqlite3");
#endif
    auto controller = new Controller(db);
    controller->start();
    delete controller;
    return 0;
}

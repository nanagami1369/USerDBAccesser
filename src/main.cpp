#include "Controller.h"
#include "MemoryUserDB.h"
#include "SQLiteUserDB.h"
#include "User.h"
#include "UserDB.h"
int main(int argc, char const *argv[]) {
    UserDB *db;
    //コンパイラーからのマクロ定義で動作を切り替える
    if (DB_TYPE == "memory") {
        db = new MemoryUserDB();

    } else if (DB_TYPE == "sqlite3") {
        db = new SQLiteUserDB();
    }
    auto controller = new Controller(db);
    controller->start();
    delete controller;
    return 0;
}

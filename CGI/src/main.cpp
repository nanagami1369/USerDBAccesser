#ifdef MEMORY_DB
#include "MemoryUserDB.h"
#elif SQLITE3_DB
#include "SQLiteUserDB.h"
#endif
#include "CGIController.h"
#include <cstdio>
int main() {
    UserDB *db;
#ifdef MEMORY_DB
    db = new MemoryUserDB();
#elif SQLITE3_DB
    db = new SQLiteUserDB("/db/user.sqlite3");
#endif
    auto controller = new CGIController(db);
    controller->exec();
}

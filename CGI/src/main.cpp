#ifdef MEMORY_DB
#include "MemoryUserDB.h"
#elif SQLITE3_DB
#include "SQLiteUserDB.h"
#elif MYSQL_DB
#include "MySQLUserDB.h"
#endif
#include "CGIController.h"
#include <cstdio>

static std::string getenvTostring(const char *name) {
    auto envPtr = std::getenv(name);
    if (envPtr == nullptr) {
        std::string message;
        throw std::invalid_argument("この名前の環境変数を取得できませんでした：" + std::string(name));
    }
    return std::string(envPtr);
}

int main() {
    UserDB *db;
#ifdef MEMORY_DB
    db = new MemoryUserDB();
#elif SQLITE3_DB
    db = new SQLiteUserDB("/db/user.sqlite3");
#elif MYSQL_DB
    db = new MySQLUserDB(
        getenvTostring("MYSQL_HOST"),
        getenvTostring("MYSQL_USER"),
        getenvTostring("MYSQL_PASSWORD"),
        getenvTostring("MYSQL_DATABASE"));
#endif
    auto controller = new CGIController(db);
    controller->exec();
}

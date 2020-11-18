#include "UserDB.h"
#include <string>

class SQLiteUserDB : public UserDB {
  private:
    std::string dbName = "user.sqlite3";
    void addInternalDatabase(
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) override;
    User searchByIdInternalDatabase(const uint id) override;
    std::vector<User> searchByNameInternalDatabase(const std::string name) override;
    std::vector<User> searchByAvailInternalDatabase(const bool avail) override;
    void removeInternalDatabase(const uint id) override;
    void updateInternalDatabase(
        const uint updateUserId,
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) override;

  public:
    void WriterAllUserToConsole() override;
    uint GetLastId() override;
    SQLiteUserDB();
};

#include <UserDB.h>
#include <string>

class SQLiteUserDB : public UserDB {
  private:
    std::string dbName = "user.sqlite3";
    void addInternalDatabase(
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) override;
    User searchInternalDatabase(const std::string id) override;
    void removeInternalDatabase(const std::string id) override;
    void updateInternalDatabase(
        const std::string updateUserId,
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) override;

  public:
    void WriterAllUserToConsole() override;

    SQLiteUserDB();
};

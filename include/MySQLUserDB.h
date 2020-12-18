#include "UserDB.h"
#include <string>

class MySQLUserDB : public UserDB {
  private:
    std::string mysqlHost;
    std::string mysqlUser;
    std::string mysqlPassword;
    std::string mysqlDatabase;

    void addInternalDatabase(
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) override;
    User searchByIdInternalDatabase(const uint id) override;
    std::vector<User> searchByNameInternalDatabase(const std::string name) override;
    std::vector<User> searchByAvailInternalDatabase(const bool avail) override;
    std::vector<User> searchByLevelInternalDatabase(const t_Level level) override;
    void removeInternalDatabase(const uint id) override;
    void updateInternalDatabase(
        const uint updateUserId,
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) override;

  public:
    std::vector<User> GetAllUserData() override;
    uint GetLastId() override;
    MySQLUserDB(
        const std::string host,
        const std::string user,
        const std::string password,
        const std::string database);
};

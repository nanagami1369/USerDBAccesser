#include "UserDB.h"
#include <string>

class MemoryUserDB : public UserDB {
  private:
    std::vector<std::shared_ptr<User>> Memory;
    uint index = 0;
    int searchIndex(const std::string id);
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
    std::string GetLastId() override;
    MemoryUserDB();
};

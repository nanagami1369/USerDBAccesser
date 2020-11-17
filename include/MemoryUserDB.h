#include "UserDB.h"
#include <string>

class MemoryUserDB : public UserDB {
  private:
    std::vector<std::shared_ptr<User>> Memory;
    uint index = 0;
    int searchIndex(const uint id);
    void addInternalDatabase(
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) override;
    User searchByIdInternalDatabase(const uint id) override;
    std::vector<User> searchByNameInternalDatabase(const std::string name) override;
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
    MemoryUserDB();
};

#pragma once
#include "UserDB.h"
#include <nlohmann/json.hpp>

class CGIController {
  private:
    UserDB *db;
    std::vector<std::string> getPathInfo();
    std::string getPostData();
    void execMethod(const std::string &methodName, const nlohmann::json &args);
    void getAllUser();

  public:
    CGIController(UserDB *db);
    ~CGIController();
    void exec();
};

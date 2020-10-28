#include "User.h"
#include <iostream>
static std::string t_LevelToString(t_Level level) {
    switch (level) {
    case ADMIN:
        return "ADMIN";
    case PREM:
        return "PREM";
    case GEN:
        return "GEN";
    case TRY:
        return "TRY";
    default:
        return "";
    }
}

User::User(std::string id,
           std::string name,
           std::string pass,
           bool avail,
           t_Level level) {
    this->ID = id;
    this->Name = name;
    this->Pass = pass;
    this->Avail = avail;
    this->Level = level;
}
std::string User::toString() {
    auto pass = "********";
    auto avail = Avail ? "true" : "false";
    auto t_Level = t_LevelToString(Level);
    return ID + "|" + Name + "|" + pass + "|" + avail + "|" + t_Level;
}

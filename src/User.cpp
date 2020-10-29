#include "User.h"
#include "t_level.h"
#include <iostream>
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
    auto level = t_LevelToString(Level);
    return ID + "|" + Name + "|" + pass + "|" + avail + "|" + level;
}

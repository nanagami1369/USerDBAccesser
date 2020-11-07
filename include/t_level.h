#pragma once
#include <string>
enum t_Level {
    ADMIN,
    PREM,
    GEN,
    TRY
};

std::string t_LevelToString(t_Level level);
t_Level intToT_Level(int level);

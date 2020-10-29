#pragma once
enum t_Level {
    ADMIN,
    PREM,
    GEN,
    TRY
};

std::string t_LevelToString(t_Level level);

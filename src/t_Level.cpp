#include "t_level.h"
#include <stdexcept>
#include <string>

std::string t_LevelToString(t_Level level) {
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

t_Level intToT_Level(int level) {
    switch (level) {
    case 0:
        return ADMIN;
    case 1:
        return PREM;
    case 2:
        return GEN;
    case 3:
        return TRY;
    default:
        throw std::invalid_argument("そのような権限はありません");
    }
}

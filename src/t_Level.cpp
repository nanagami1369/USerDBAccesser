#include <string>
#include "t_level.h"
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

#include "getHashPassWord.h"
#include <crypt.h>
#include <stdexcept>

static const char *SHA_512 = "$6$";

std::string getHashPassWord(std::string pass) {
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        throw std::runtime_error("パスワードのハッシュ化に失敗しました。");
    }
    srandom(ts.tv_nsec ^ ts.tv_sec);
    auto r = random();
    std::string salt = SHA_512 + std::to_string(r);
    struct crypt_data data;
    data.initialized = 0;
    if (crypt_r(pass.c_str(), salt.c_str(), &data) == NULL) {
        throw std::runtime_error("パスワードのハッシュ化に失敗しました。");
    };
    return std::string(data.keysched);
}

#include "UserDB.h"
#include "ValidationException.h"
#include <crypt.h>
const char *UserDB::SHA_512 = "$6$";

void UserDB::checkIdValidation(const uint id) {
    // 何もしない。必要になったら追加する。
}

void UserDB::checkNameValidation(const std::string name) {
    if (name.empty() || name[0] == '\n') {
        throw ValidationException("名前が空文字です");
    }
}

void UserDB::checkPassWordValidation(const std::string rowPass) {
    if (rowPass.empty() || rowPass[0] == 0) {
        throw ValidationException("パスワードが空文字です");
    }
    if (rowPass.length() < 8) {
        throw ValidationException("パスワードは8文字以上です");
    }
}

std::string UserDB::createHashPassWord(const std::string rowPass, const char *HashMode) {
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
        throw std::runtime_error("パスワードのハッシュ化に失敗しました。");
    }
    srandom(ts.tv_nsec ^ ts.tv_sec);
    auto r = random();
    std::string salt = SHA_512 + std::to_string(r);
    struct crypt_data data;
    data.initialized = 0;
    if (crypt_r(rowPass.c_str(), salt.c_str(), &data) == NULL) {
        throw std::runtime_error("パスワードのハッシュ化に失敗しました。");
    };
    return std::string(data.keysched);
}

void UserDB::checkAddValidation(
    const std::string name,
    const std::string rowPass,
    const bool avail,
    const t_Level level) {
    checkNameValidation(name);
    checkPassWordValidation(rowPass);
}

void UserDB::add(
    const std::string name,
    const std::string rowPass,
    const bool avail,
    const t_Level level) {
    checkAddValidation(name, rowPass, avail, level);
    auto hashedPass = createHashPassWord(rowPass, SHA_512);
    addInternalDatabase(name, hashedPass, avail, level);
}

void UserDB::checkSearchByIdValidation(const uint id) {
    checkIdValidation(id);
}

User UserDB::searchById(const uint id) {
    checkSearchByIdValidation(id);
    return searchByIdInternalDatabase(id);
}

void UserDB::checkSearchByNameValidation(const std::string name) {
    checkNameValidation(name);
}

std::vector<User> UserDB::searchByName(const std::string name) {
    checkSearchByNameValidation(name);
    return searchByNameInternalDatabase(name);
}

std::vector<User> UserDB::searchByAvail(const bool avail) {
    return searchByAvailInternalDatabase(avail);
}

void UserDB::checkRemoveValidation(const uint id) {
    checkIdValidation(id);
}

void UserDB::remove(const uint id) {
    checkRemoveValidation(id);
    removeInternalDatabase(id);
}

void UserDB::checkUpdateValidation(
    const uint updateUserId,
    const std::string name,
    const std::string rowPass,
    const bool avail,
    const t_Level level) {
    checkIdValidation(updateUserId);
    checkNameValidation(name);
    // rowPassが空文字の場合はパスワードを変更しないと判断
    if (rowPass.empty()) {
        return;
    }
    checkPassWordValidation(rowPass);
}

void UserDB::update(
    const uint updateUserId,
    const std::string name,
    const std::string rowPass,
    const bool avail,
    const t_Level level) {
    checkUpdateValidation(updateUserId, name, rowPass, avail, level);
    // rowPassが空文字の場合はパスワードを変更しないと判断
    std::string hashedPass;
    if (!rowPass.empty()) {
        hashedPass = createHashPassWord(rowPass, SHA_512);
    }
    updateInternalDatabase(updateUserId, name, hashedPass, avail, level);
}

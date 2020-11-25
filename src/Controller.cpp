#include "Controller.h"
#include "PromptModule.h"
#include "UserDB.h"
#include "ValidationException.h"
#include <iostream>
#include <regex>
#include <termios.h>
#include <unistd.h>

Controller::Controller(UserDB *db) {
    this->db = db;
}

Controller::~Controller() {
    delete db;
}

uint Controller::InputIdPrompt() {
    uint lastId = -1;
    try {
        lastId = db->GetLastId();
    } catch (const std::range_error &e) {
        throw;
    }
    while (true) {
        try {
            return Prompt::inputNumberPrompt("IDを入力して下さい", 0, lastId);
        } catch (const ValidationException &e) {
            std::cerr << e.what() << '\n';
        }
    }
}

void Controller::addUser() {
    std::cout << "ユーザーを追加します" << '\n';
    // 名前入力
    std::string name = "";
    while (true) {
        try {
            name = Prompt::inputStringPrompt("名前を入力して下さい");
            break;
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << '\n';
        }
    }
    // パスワード入力
    std::string pass = Prompt::inputPasswordPrompt();
    // 有効、無効
    uint8_t availNumber;
    constexpr int availMenuLength = 2;
    const char *availMenu[availMenuLength] = {"有効", "無効"};
    availNumber = Prompt::selectMenuPrompt("どの状態で作成しますか", availMenu, availMenuLength);
    auto avail = availNumber == 0 ? true : false;
    // 権限
    uint8_t levelNumber;
    constexpr int levelMenuLength = 4;
    const char *levelMenu[levelMenuLength] = {"ADMIN", "PREM", "GEN", "TRY"};
    levelNumber = Prompt::selectMenuPrompt("権限を決定して下さい", levelMenu, levelMenuLength);
    auto level = intToT_Level(levelNumber);
    std::cout << "==================================" << '\n';
    std::cout << "氏名：" << name << '\n';
    std::cout << "パスワード：********" << '\n';
    std::cout << "アカウントの状態：" << (avail ? "有効" : "無効") << '\n';
    std::cout << "権限：" << t_LevelToString(level) << '\n';
    std::cout << "==================================" << '\n';
    const char *isSaveUserMenu[] = {"追加する", "いいえ"};
    auto isSave = Prompt::yesOrNoPrompt("以下のアカウントを追加しますか？", isSaveUserMenu);
    if (isSave) {
        db->add(name, pass, avail, level);
        std::cout << "ユーザーの追加に成功しました" << '\n';
        return;
    }
    std::cout << "ユーザーの追加を止めました" << '\n';
    return;
}

void Controller::showUser() {
    std::cout << "ユーザーの一覧を表示します" << '\n';
    db->WriterAllUserToConsole();
    Prompt::pausePrompt();
}

void Controller::removeUser() {
    uint id = -1;
    try {
        id = InputIdPrompt();
    } catch (const std::range_error &e) {
        std::cerr << e.what() << '\n';
        return;
    }
    User user(0, "", "", true, ADMIN);
    try {
        user = db->searchById(id);
        std::cout << "アカウントが見つかりました" << '\n';
        std::cout << "==================================" << '\n';
        std::cout << user.toString() << '\n';
        std::cout << "==================================" << '\n';
    } catch (const std::range_error &e) {
        std::cerr << "アカウントが見つかりませんでした" << '\n';
        return;
    } catch (const ValidationException &e) {
        std::cerr << e.what() << '\n';
    }
    constexpr int yesOrNoMenuLength = 2;
    const char *yesOrNoMenu[yesOrNoMenuLength] = {"削除する", "いいえ"};
    auto isRemove = Prompt::yesOrNoPrompt("このアカウントを削除しますか？", yesOrNoMenu);
    if (isRemove) {
        try {
            db->remove(user.ID);
            std::cout << "アカウントを削除しました。" << '\n';
            return;
        } catch (const ValidationException &e) {
            std::cerr << e.what() << '\n';
        }
        std::cout << "アカウントを削除しませんでした" << '\n';
    }
}

void Controller::changeAvail() {
    uint id = -1;
    try {
        id = InputIdPrompt();
    } catch (const std::range_error &e) {
        std::cerr << e.what() << '\n';
        return;
    }
    User user(0, "", "", true, ADMIN);
    try {
        user = db->searchById(id);
        std::cout << "アカウントが見つかりました" << '\n';
        std::cout << "==================================" << '\n';
        std::cout << user.toString() << '\n';
        std::cout << "==================================" << '\n';
    } catch (const std::range_error &e) {
        std::cerr << "アカウントが見つかりませんでした" << '\n';
        return;
    } catch (const ValidationException &e) {
        std::cerr << e.what() << '\n';
    }
    if (user.Avail) {
        std::cout << "このアカウントは有効です" << '\n';
        const char *unEnableYesOrNoMenu[] = {"無効化する", "いいえ"};
        auto isUnEnable = Prompt::yesOrNoPrompt("無効化しますか？", unEnableYesOrNoMenu);
        if (isUnEnable) {
            try {
                db->update(user.ID, user.Name, "", false, user.Level);
            } catch (const ValidationException &e) {
                std::cerr << e.what() << '\n';
            }
            std::cout << "アカウントを無効化しました" << '\n';
            return;
        }
        std::cout << "アカウントを無効化しませんでした" << '\n';
    } else {
        std::cout << "このアカウントは無効化されています" << '\n';
        const char *enableYesOrNoMenu[] = {"有効にする", "いいえ"};
        auto isEnable = Prompt::yesOrNoPrompt("有効にしますか？", enableYesOrNoMenu);
        if (isEnable) {
            try {
                db->update(user.ID, user.Name, "", true, user.Level);
            } catch (const ValidationException &e) {
                std::cerr << e.what() << '\n';
            }
            std::cout << "アカウントを有効にしました" << '\n';
            return;
        }
        std::cout << "アカウントを有効にしませんでした" << '\n';
    }
}

void Controller::searchUser() {
    std::cout << "検索を開始します" << '\n';
    constexpr int searchModeMenuLength = 4;
    const char *searchModeMenu[searchModeMenuLength] = {
        "IDで検索する",
        "名前で検索する",
        "アカウントの状態で検索する",
        "権限で検索する"};
    auto searchMode = Prompt::selectMenuPrompt("検索モードを指定して下さい", searchModeMenu, searchModeMenuLength);
    switch (searchMode) {
    case 0:
        searchUserById();
        break;
    case 1:
        searchUserByName();
        break;
    case 2:
        searchUserByAvail();
        break;
    case 3:
        searchUserByLevel();
        break;
    default:
        break;
    }
}

void Controller::searchUserById() {
    std::cout << "IDで検索します" << '\n';
    uint id = -1;
    try {
        id = InputIdPrompt();
    } catch (const std::range_error &e) {
        std::cerr << e.what() << '\n';
        return;
    }
    try {
        auto user = db->searchById(id);
        std::cout << "アカウントが見つかりました" << '\n';
        std::cout << "==================================" << '\n';
        std::cout << user.toString() << '\n';
        std::cout << "==================================" << '\n';
    } catch (const std::range_error &e) {
        std::cerr << "アカウントが見つかりませんでした" << '\n';
        return;
    } catch (const ValidationException &e) {
        std::cerr << e.what() << '\n';
    }
}

void Controller::searchUserByName() {
    std::cout << "名前で検索します" << '\n';
    std::string name = "";
    while (true) {
        try {
            name = Prompt::inputStringPrompt("名前を入力して下さい");
            break;
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << '\n';
        }
    }
    try {
        auto users = db->searchByName(name);
        std::cout << "アカウントが見つかりました" << '\n';
        std::cout << "==================================" << '\n';
        for (auto &user : users) {
            std::cout << user.toString() << '\n';
        }
        std::cout << "==================================" << '\n';
    } catch (const std::range_error &e) {
        std::cerr << "アカウントが見つかりませんでした" << '\n';
    }
}
void Controller::searchUserByAvail() {
    std::cout << "アカウントの状態で検索します" << '\n';
    constexpr int searchAvailModeMenuLength = 2;
    const char *searchAvailModeMenu[searchAvailModeMenuLength] = {"有効", "無効"};
    auto searchAvailModeNumber = Prompt::selectMenuPrompt("どちらの状態を検索しますか？", searchAvailModeMenu, searchAvailModeMenuLength);
    auto searchAvailMode = searchAvailModeNumber == 0 ? true : false;
    if (searchAvailMode) {
        std::cout << "有効なアカウントを検索します" << '\n';
    } else {
        std::cout << "無効なアカウントを検索します" << '\n';
    }
    try {
        auto users = db->searchByAvail(searchAvailMode);
        std::cout << "アカウントが見つかりました" << '\n';
        std::cout << "==================================" << '\n';
        for (auto &user : users) {
            std::cout << user.toString() << '\n';
        }
        std::cout << "==================================" << '\n';
    } catch (const std::range_error &e) {
        std::cerr << "アカウントが見つかりませんでした" << '\n';
    }
}

void Controller::searchUserByLevel() {
    std::cout << "権限で検索します" << '\n';
    constexpr int searchLevelModeMenuLength = 4;
    const char *searchLevelModeMenu[searchLevelModeMenuLength] = {"ADMIN", "PREM", "GEN", "TRY"};
    auto searchLevel = Prompt::selectMenuPrompt("どちらの状態を検索しますか？", searchLevelModeMenu, searchLevelModeMenuLength);
    auto level = intToT_Level(searchLevel);
    std::cout << "この権限を持つアカウントを検索します-->" << t_LevelToString(level) << '\n';
    try {
        auto users = db->searchByLevel(level);
        std::cout << "アカウントが見つかりました" << '\n';
        std::cout << "==================================" << '\n';
        for (auto &user : users) {
            std::cout << user.toString() << '\n';
        }
        std::cout << "==================================" << '\n';
    } catch (const std::range_error &e) {
        std::cerr << "アカウントが見つかりませんでした" << '\n';
    }
}

void Controller::updateUser() {
    std::cout << "ユーザーの情報を変更します" << '\n';
    uint id = -1;
    try {
        id = InputIdPrompt();
    } catch (const std::range_error &e) {
        std::cerr << e.what() << '\n';
        return;
    }
    User user(0, "", "", true, ADMIN);
    try {
        user = db->searchById(id);
        std::cout << "アカウントが見つかりました" << '\n';
        std::cout << "==================================" << '\n';
        std::cout << user.toString() << '\n';
        std::cout << "==================================" << '\n';
    } catch (const std::range_error &e) {
        std::cerr << "アカウントが見つかりませんでした" << '\n';
        return;
    } catch (const ValidationException &e) {
        std::cerr << e.what() << '\n';
    }
    User changedUser(user.ID, user.Name, "", user.Avail, user.Level);
    // 名前変更
    const char *isChangedNameMenu[] = {"名前を変更する", "いいえ"};
    auto isChangedName = Prompt::yesOrNoPrompt("名前を変更しますか？", isChangedNameMenu);
    if (isChangedName) {
        while (true) {
            try {
                changedUser.Name = Prompt::inputStringPrompt("名前を入力して下さい");
                break;
            } catch (const std::invalid_argument &e) {
                std::cerr << e.what() << '\n';
            }
        };
    }
    // パスワード変更
    const char *isChangedPassWordMenu[] = {"パスワードを変更する", "いいえ"};
    auto isChangedPassWord = Prompt::yesOrNoPrompt("パスワードを変更しますか？", isChangedPassWordMenu);
    if (isChangedPassWord) {
        changedUser.Pass = Prompt::inputPasswordPrompt();
    }
    // 有効、無効
    if (user.Avail) {
        std::cout << "このアカウントは有効です" << '\n';
        const char *unEnableYesOrNoMenu[] = {"無効化する", "いいえ"};
        auto isUnEnable = Prompt::yesOrNoPrompt("無効化しますか？", unEnableYesOrNoMenu);
        if (isUnEnable) {
            changedUser.Avail = false;
        }
    } else {
        std::cout << "このアカウントは無効化されています" << '\n';
        const char *enableYesOrNoMenu[] = {"有効にする", "いいえ"};
        auto isEnable = Prompt::yesOrNoPrompt("有効にしますか？", enableYesOrNoMenu);
        if (isEnable) {
            changedUser.Avail = true;
        }
    }
    // 権限
    const char *isChangedLevelMenu[] = {"権限を変更する", "いいえ"};
    auto isChangedLevel = Prompt::yesOrNoPrompt("権限を変更しますか？", isChangedLevelMenu);
    if (isChangedLevel) {
        constexpr int whichLevelModeMenuLength = 4;
        const char *whichLevelModeMenu[whichLevelModeMenuLength] = {"ADMIN", "PREM", "GEN", "TRY"};
        auto whichLevel = Prompt::selectMenuPrompt("どちらの状態を検索しますか？", whichLevelModeMenu, whichLevelModeMenuLength);
        auto level = intToT_Level(whichLevel);
        changedUser.Level = level;
    }
    // 確認
    std::cout << "==================================" << '\n';
    std::cout << "氏名：" << user.Name << " >> " << changedUser.Name << '\n';
    std::cout << "パスワード： "
              << (changedUser.Pass.empty() ? "変更なし" : "変更あり")
              << '\n';
    std::cout << "アカウントの状態："
              << (user.Avail ? "有効" : "無効") << " >> " << (changedUser.Avail ? "有効" : "無効")
              << '\n';
    std::cout << "権限："
              << t_LevelToString(user.Level) << " >> " << t_LevelToString(changedUser.Level)
              << '\n';
    std::cout << "==================================" << '\n';
    const char *isSaveUserMenu[] = {"変更する", "いいえ"};
    auto isSave = Prompt::yesOrNoPrompt("以下の情報に変更しますか？", isSaveUserMenu);
    if (isSave) {
        try {
            db->update(
                changedUser.ID,
                changedUser.Name,
                changedUser.Pass,
                changedUser.Avail,
                changedUser.Level);
            std::cout << "情報の更新に成功しました" << '\n';
        } catch (const ValidationException &e) {
            std::cerr << e.what() << '\n';
        }
        return;
    }
    std::cout << "ユーザーの追加を止めました" << '\n';
    return;
}

void Controller::start() {
    std::cout << "UserDBへようこそ!!" << '\n';

    int controlNumber = 100;
    constexpr int mainMenuLength = 7;
    const char *mainMenu[mainMenuLength] = {
        "終了",
        "ユーザーの追加",
        "ユーザーの一覧",
        "ユーザーの削除",
        "ユーザーの無効化、有効化",
        "ユーザーの検索",
        "ユーザーの情報を変更する"};
    while (controlNumber != 0) {
        controlNumber = Prompt::selectMenuPrompt("何をしますか？", mainMenu, mainMenuLength);
        switch (controlNumber) {
        case 0:
            std::cout << "さようなら!" << '\n';
            break;
        case 1:
            addUser();
            break;
        case 2:
            showUser();
            break;
        case 3:
            removeUser();
            break;
        case 4:
            changeAvail();
            break;
        case 5:
            searchUser();
            break;
        case 6:
            updateUser();
            break;
        default:
            std::cerr << "無効な数字です" << '\n';
            break;
        }
    }
}

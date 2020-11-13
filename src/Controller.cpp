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

void Controller::addUser() {
    std::cout << "ユーザーを追加します" << std::endl;
    // 名前入力
    std::string name = "";
    while (true) {
        try {
            name = Prompt::inputStringPrompt("名前を入力して下さい");
            break;
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
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
    db->add(name, pass, avail, level);
    std::cout << "ユーザーの追加に成功しました" << std::endl;
}

void Controller::removeUser() {
    int id = -1;
    int lastId = -1;
    try {
        lastId = std::stoi(db->GetLastId());
    } catch (const std::range_error &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    while (true) {
        try {
            id = Prompt::inputNumberPrompt("IDを入力して下さい", 0, lastId);
            break;
        } catch (const ValidationException &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    try {
        auto user = this->db->search(std::to_string(id));
        constexpr int yesOrNoMenuLength = 2;
        const char *yesOrNoMenu[yesOrNoMenuLength] = {"はい", "いいえ"};
        std::cout << "アカウントが見つかりました" << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << user.toString() << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "このアカウントを削除します" << std::endl;
        auto select = Prompt::selectMenuPrompt("よろしいですか？", yesOrNoMenu, yesOrNoMenuLength);
        switch (select) {
        case 0:
            db->remove(user.ID);
            std::cout << "アカウントを削除しました。" << std::endl;
            return;
        case 1:
            std::cout << "アカウントを削除しませんでした" << std::endl;
            return;
        default:
            //この値は出るはずが無いので何もしない
            break;
        }
    } catch (const ValidationException &e) {
        std::cerr << e.what() << std::endl;
    } catch (const std::range_error &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Controller::changeAvail() {
    int id = -1;
    int lastId = -1;
    try {
        lastId = std::stoi(db->GetLastId());
    } catch (const std::range_error &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    while (true) {
        try {
            id = Prompt::inputNumberPrompt("IDを入力して下さい", 0, lastId);
            break;
        } catch (const ValidationException &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    try {
        auto user = db->search(std::to_string(id));
        constexpr int yesOrNoMenuLength = 2;
        const char *yesOrNoMenu[yesOrNoMenuLength] = {"はい", "いいえ"};
        std::cout << "アカウントが見つかりました" << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << user.toString() << std::endl;
        std::cout << "==================================" << std::endl;
        if (user.Avail) {
            std::cout << "このアカウントは有効です" << std::endl;
            auto select = Prompt::selectMenuPrompt("無効化しますか？", yesOrNoMenu, yesOrNoMenuLength);
            switch (select) {
            case 0:
                try {
                    db->update(user.ID, user.Name, "", false, user.Level);
                } catch (const std::range_error &e) {
                    std::cerr << "アカウントが見つかりませんでした" << std::endl;
                } catch (const ValidationException &e) {
                    std::cerr << e.what() << std::endl;
                }
                std::cout << "アカウントを無効化しました" << std::endl;
                return;
            case 1:
                std::cout << "アカウントを無効化しませんでした" << std::endl;
                return;
            default:
                //この値は出るはずが無いので何もしない
                break;
            }
        } else {
            std::cout << "このアカウントは無効化されています" << std::endl;
            auto select = Prompt::selectMenuPrompt("有効にしますか？", yesOrNoMenu, yesOrNoMenuLength);
            switch (select) {
            case 0:
                try {
                    db->update(user.ID, user.Name, "", true, user.Level);
                } catch (const std::range_error &e) {
                    std::cerr << "アカウントが見つかりませんでした" << std::endl;
                } catch (const ValidationException &e) {
                    std::cerr << e.what() << std::endl;
                }
                std::cout << "アカウントを有効にしました" << std::endl;
                return;
            case 1:
                std::cout << "アカウントを有効にしませんでした" << std::endl;
                return;
            default:
                //この値は出るはずが無いので何もしない
                break;
            }
        }
    } catch (const std::range_error &e) {
        std::cerr << "アカウントが見つかりませんでした" << std::endl;
        return;
    } catch (const ValidationException &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Controller::start() {
    std::cout << "UserDBへようこそ!!" << std::endl;

    int controlNumber = 100;
    constexpr int mainMenuLength = 5;
    const char *mainMenu[mainMenuLength] = {
        "終了",
        "ユーザーの追加",
        "ユーザーの一覧",
        "ユーザーの削除",
        "ユーザーの無効化、有効化"};
    while (controlNumber != 0) {
        controlNumber = Prompt::selectMenuPrompt("何をしますか？", mainMenu, mainMenuLength);
        switch (controlNumber) {
        case 0:
            std::cout << "さようなら!" << std::endl;
            break;
        case 1:
            addUser();
            break;
        case 2:
            std::cout << "ユーザーの一覧を表示します" << std::endl;
            db->WriterAllUserToConsole();
            break;
        case 3:
            removeUser();
            break;
        case 4:
            changeAvail();
            break;
        default:
            std::cerr << "無効な数字です" << std::endl;
            break;
        }
    }
}

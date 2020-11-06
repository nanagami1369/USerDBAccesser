#include "Controller.h"
#include "UserDB.h"
#include <iostream>
#include <regex>
#include <termios.h>
#include <unistd.h>
static int StringToIntForStdIO() {
    auto number = -1;
    std::string value = "";
    getline(std::cin, value);
    try {
        number = std::stoi(value);
    } catch (const std::invalid_argument e) {
        std::cerr << "入力された値を数字に変換できませんでした" << std::endl;
        return -1;
    } catch (const std::out_of_range) {
        //範囲外の数字として使用する側でエラーチェックするので何もしない
        return -1;
    }
    return number;
}

static const char *pattern = "^\\$[56]\\$";
static const std::regex re(pattern);

Controller::Controller(UserDB *db) {
    this->db = db;
}

Controller::~Controller() {
    delete db;
}

std::string Controller::readName() {
    std::string name = "";
    while (true) {
        std::cout << "名前を入力して下さい>";
        std::getline(std::cin, name);
        if (name.empty() || name.length() == 0) {
            std::cerr << "名前が空文字です" << std::endl;
        } else {
            return name;
        }
    }
}

std::string Controller::readPassward() {
    std::string pass = "";
    std::string confirmationPass = "";
    struct termios term;
    struct termios save;
    while (true) {
        std::cout << "パスワードを入力して下さい>";
        tcgetattr(STDIN_FILENO, &term);
        save = term;
        term.c_lflag &= ~ECHO;
        // パスワード入力時はターミナルの応答を消す
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        std::getline(std::cin, pass);
        tcsetattr(STDIN_FILENO, TCSANOW, &save);
        putchar('\n');
        if (pass.empty() || pass.length() == 0) {
            std::cerr << "パスワードが空文字です" << std::endl;
            continue;
        }
        if (pass.length() < 8) {
            std::cerr << "パスワードは8文字以上にしてください" << std::endl;
            continue;
        }
        if (std::regex_search(pass, re)) {
            // ハッシュ化されたパスワードかどうかの判定に、
            //「$5$」、「$6$」が先頭についてるかで判別するために禁止
            std::cerr << "パスワードの先頭には、「$5$」、「$6$」を使えません" << std::endl;
            continue;
        }
        while (true) {
            std::cout << "もう一度パスワードを入力して下さい>";
            tcgetattr(STDIN_FILENO, &term);
            save = term;
            term.c_lflag &= ~ECHO;
            // パスワード入力時はターミナルの応答を消す
            tcsetattr(STDIN_FILENO, TCSANOW, &term);
            std::getline(std::cin, confirmationPass);
            tcsetattr(STDIN_FILENO, TCSANOW, &save);
            putchar('\n');
            if (confirmationPass.empty() || confirmationPass.length() == 0) {
                std::cerr << "パスワードが空文字です" << std::endl;
                continue;
            }
            if (confirmationPass.length() < 8) {
                std::cerr << "パスワードは8文字以上にしてください" << std::endl;
                continue;
            }
            if (std::regex_search(pass, re)) {
                // ハッシュ化されたパスワードかどうかの判定に、
                //「$5$」、「$6$」が先頭についてるかで判別するために禁止
                std::cerr << "パスワードの先頭には、「$5$」、「$6$」を使えません" << std::endl;
                continue;
            }
            break;
        }
        // パスワードが一致しなかった場合は、パスワードの入力をやり直す
        if (pass != confirmationPass) {
            std::cerr << "パスワードが一致しませんでした。" << std::endl;
            continue;
        }
        break;
    }
    return pass;
}

bool Controller::readIsAvail() {
    int availNumber;
    while (true) {
        std::cout << "無効な状態で作成しますか (0:有効 1:無効)>";
        availNumber = StringToIntForStdIO();
        if (availNumber != 0 && availNumber != 1) {
            std::cerr << "無効な値が入力されました" << std::endl;
        } else {
            return availNumber == 0 ? true : false;
        }
    }
}

t_Level Controller::readLevel() {
    int LevelNumber;
    while (true) {
        std::cout << "権限を決めて下さい (0:ADMIN 1:PREM 2:GEN 3:TRY)>";
        LevelNumber = StringToIntForStdIO();
        t_Level level;
        switch (LevelNumber) {
        case 0:
            return ADMIN;
        case 1:
            return PREM;
        case 2:
            return GEN;
        case 3:
            return TRY;
        default:
            std::cerr << "無効な値が入力されました" << std::endl;
        }
    }
}

void Controller::removeUser() {
    std::string id = "";
    while (true) {
        std::cout << "IDを入力して下さい>";
        std::getline(std::cin, id);
        if (id.empty() || id.length() == 0) {
            std::cerr << "IDが空文字です" << std::endl;
            continue;
        }
        break;
    }
    try {
        auto user = this->db->search(id);
        std::cout << "アカウントが見つかりました" << std::endl;
        while (true) {
            std::cout << "==================================" << std::endl;
            std::cout << user.toString() << std::endl;
            std::cout << "==================================" << std::endl;

            std::cout << "このアカウントを削除します" << std::endl;
            std::cout << "よろしいですか？ (0:はい 1:いいえ)>";
            auto select = StringToIntForStdIO();
            switch (select) {
            case 0:
                db->remove(user.ID);
                std::cout << "アカウントを削除しました。" << std::endl;
                return;
            case 1:
                std::cout << "アカウントを削除しませんでした" << std::endl;
                return;
            default:
                std::cerr << "無効な数字です" << std::endl;
                continue;
            }
            break;
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Controller::start() {
    std::cout << "UserDBへようこそ!!" << std::endl;

    int controlNumber = 100;
    while (controlNumber != 0) {
        std::cout << "何をしますか (0:終了 1:ユーザーの追加 2:ユーザー一覧 3:ユーザー削除)>";
        controlNumber = StringToIntForStdIO();
        std::string name = "";
        std::string pass = "";
        bool avail = "";
        t_Level level = ADMIN;
        switch (controlNumber) {
        case 0:
            std::cout << "さようなら!" << std::endl;
            break;
        case 1:
            std::cout << "ユーザーを追加します" << std::endl;
            name = readName();
            pass = readPassward();
            avail = readIsAvail();
            level = readLevel();
            db->add(name, pass, avail, level);
            std::cout << "ユーザーの追加に成功しました" << std::endl;
            break;
        case 2:
            std::cout << "ユーザーの一覧を表示します" << std::endl;
            db->WriterAllUserToConsole();
            break;
        case 3:
            removeUser();
            break;
        default:
            std::cerr << "無効な数字です" << std::endl;
            break;
        }
    }
}

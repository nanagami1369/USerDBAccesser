#include "Controller.h"
#include "UserDB.h"
#include <iostream>
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

Controller::Controller(UserDB *db) {
    this->db = db;
    db->add("田中", "1243251", true, ADMIN);
    db->add("中田", "24332243", true, TRY);
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
    while (true) {
        std::cout << "パスワードを入力して下さい>";
        std::getline(std::cin, pass);
        if (pass.empty() || pass.length() == 0) {
            std::cerr << "パスワードが空文字です" << std::endl;
        } else if (pass.length() < 8) {
            std::cerr << "パスワードは8文字以上にしてください" << std::endl;
        } else {
            return pass;
        }
    }
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

void Controller::start() {
    std::cout << "UserDBへようこそ!!" << std::endl;

    int controlNumber = 100;
    while (controlNumber != 0) {
        std::cout << "何をしますか (0:終了 1:ユーザーの追加 2:ユーザー一覧)>";
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
        default:
            std::cerr << "無効な数字です" << std::endl;
            break;
        }
    }
}

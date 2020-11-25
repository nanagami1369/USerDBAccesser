#include "PromptModule.h"
#include "ValidationException.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>
std::string Prompt::inputStringPrompt(const char *message) {
    std::printf("%s>", message);
    std::string value = "";
    std::getline(std::cin, value);
    if (value.empty() || value[0] == '\n') {
        throw std::invalid_argument("何も入力されてません");
    }
    return value;
}

int Prompt::inputNumberPrompt(const char *message, const int minLength, const int maxLength) {
    std::printf("%s [%d ~ %d]>", message, minLength, maxLength);
    auto number = 0;
    std::string value = "";
    getline(std::cin, value);
    try {
        number = std::stoi(value);
    } catch (const std::invalid_argument e) {
        throw std::invalid_argument("入力された値を数字に変換できませんでした");
    } catch (const std::out_of_range) {
        throw std::out_of_range("有効数字の範囲外です");
    }
    if (minLength <= number && number <= maxLength) {
        return number;
    }
    throw std::out_of_range("有効数字の範囲外です");
}

static void printSelectMenu(const char *menuItems[], const int menuItemsLength, const uint8_t selectIndex) {
    for (uint8_t index = 0; index < menuItemsLength; index++) {
        if (selectIndex == index) {
            std::printf("\e[3%d\e[4%dm%d:%s\e[m", Prompt::PromptColor::WHITE, Prompt::PromptColor::BLUE, index, menuItems[index]);
        } else {
            std::printf("%d:%s", index, menuItems[index]);
        }
        putchar('\n');
    }
}

uint8_t Prompt::selectMenuPrompt(const char *message, const char *menuItems[], const int menuItemsLength) {
    uint8_t selectIndex = 0;
    std::printf("%s [↑キー ↓キー:移動 Enter:確定]\n", message);
    printSelectMenu(menuItems, menuItemsLength, selectIndex);

    struct termios term;
    struct termios save;
    tcgetattr(STDIN_FILENO, &term);
    save = term;
    term.c_lflag &= ~ECHO;
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    char key = 0;
    while (true) {
        key = fgetc(stdin);
        //Enterなら終了
        if (key == 10) {
            std::printf("\r\e[%dA", menuItemsLength);
            for (uint8_t index = 0; index < menuItemsLength; index++) {
                std::printf("\r\e[K\n");
            }
            std::printf("\r\e[%dA", menuItemsLength);
            tcsetattr(STDIN_FILENO, TCSANOW, &save);
            std::printf("\e[3%dm✓ \e[m%sが選択されました\n", PromptColor::GREEN, menuItems[selectIndex]);
            return selectIndex;
        }
        // 数字キーで項目があるならそこまで移動
        if (48 <= key && key <= 57) {
            uint8_t selectNumber = key - 48;
            if (selectNumber < menuItemsLength) {
                selectIndex = selectNumber;
                std::printf("\r\e[%dA", menuItemsLength);
                printSelectMenu(menuItems, menuItemsLength, selectIndex);
            }
        }

        //矢印キーで移動
        if (key == 27) {
            if (fgetc(stdin) == 91) {
                switch (fgetc(stdin)) {
                case 65:
                    // ↑キー
                    if (selectIndex > 0) {
                        selectIndex--;
                    }
                    std::printf("\r\e[%dA", menuItemsLength);
                    printSelectMenu(menuItems, menuItemsLength, selectIndex);
                    break;
                case 66:
                    // ↓キー
                    if (selectIndex < menuItemsLength - 1) {
                        selectIndex++;
                    }
                    std::printf("\r\e[%dA", menuItemsLength);
                    printSelectMenu(menuItems, menuItemsLength, selectIndex);
                default:
                    break;
                }
            }
        }
    }
}

std::string Prompt::inputPasswordPrompt() {
    std::string pass = "";
    std::string confirmationPass = "";
    struct termios term;
    struct termios save;
    // パスワード入力時はターミナルの応答を消す
    tcgetattr(STDIN_FILENO, &term);
    save = term;
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    while (true) {
        std::printf("パスワードを入力して下さい>");
        std::getline(std::cin, pass);
        //応答が帰って来ないので改行をこちらからする
        std::putchar('\n');
        if (pass.empty() || pass[0] == '\n') {
            std::fprintf(stderr, "何も入力されませんでした\n");
            continue;
        }
        if (pass.length() < 8) {
            std::fprintf(stderr, "パスワードは8文字以上にして下さい\n");
            continue;
        }
        while (true) {
            std::printf("もう一度パスワードを入力して下さい>");
            std::getline(std::cin, confirmationPass);
            putchar('\n');
            if (confirmationPass.empty() || confirmationPass[0] == '\n') {
                std::fprintf(stderr, "何も入力されませんでした\n");
                continue;
            }
            break;
        }
        // パスワードが一致しなかった場合は、パスワードの入力をやり直す
        if (pass != confirmationPass) {
            std::fprintf(stderr, "パスワードが一致しませんでした\n");
            continue;
        }
        break;
    }
    // ターミナルを元に戻す
    tcsetattr(STDIN_FILENO, TCSANOW, &save);
    return pass;
}

bool Prompt::yesOrNoPrompt(const char *message, const char *yesOrNoMenu[]) {
    constexpr int yesOrNoMenuLength = 2;
    auto selectIndex = selectMenuPrompt(message, yesOrNoMenu, yesOrNoMenuLength);
    if (selectIndex == 0) {
        return true;
    }
    return false;
}

void Prompt::pausePrompt() {
    std::printf("[q:終了]");
    struct termios term;
    struct termios save;
    tcgetattr(STDIN_FILENO, &term);
    save = term;
    term.c_lflag &= ~ECHO;
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    char key = 0;
    //qキーが押されるまで待機
    while (fgetc(stdin) != 113) {
    }
    std::printf("\r\e[K");
    tcsetattr(STDIN_FILENO, TCSANOW, &save);
    return;
}

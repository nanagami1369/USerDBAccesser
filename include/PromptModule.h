/**
 * @file PromptModule.h
 * @brief 標準入力からデータを取得する
**/
#include <stdint.h>
#include <string>
namespace Prompt {

/**
 * @brief ターミナル上で表示する色
 */
namespace PromptColor {
constexpr uint8_t BLACK = 0;
constexpr uint8_t RED = 1;
constexpr uint8_t GREEN = 2;
constexpr uint8_t YELLOW = 3;
constexpr uint8_t BLUE = 4;
constexpr uint8_t MAGENTA = 5;
constexpr uint8_t CYAN = 6;
constexpr uint8_t WHITE = 7;
} // namespace PromptColor

/**
 * @brief 標準入力から文字列を取得するプロンプト
 * @param message プロンプトに表示するメッセージ
 * @return 入力された文字列
 * @attention 入力がなかった場合 ValidationException を出す
 */
std::string inputStringPrompt(const char *message);
/**
 * @brief 標準入力から文字列を取得するプロンプト
 * @param message プロンプトに表示するメッセージ
 * @param minLength 有効数字の最低値
 * @param maxLength 有効数字の最高値
 * @return 入力された数値
 * @attention 数値に変換できなかった場合 ValidationException を出す
 * @attention 有効数字の範囲外の場合 ValidationException を出す
 */
int inputNumberPrompt(const char *message, const int minLength, const int maxLength);
/**
 * @brief 選択肢から一つをユーザーに選んで貰うプロンプト
 * @note ↑キーと↓キーで項目を移動 Enterで確定
 * @param message プロンプトに表示するメッセージ
 * @param menuItem 表示する選択肢
 * @param menuItemLength menuItemの長さ
 * @return 選択されたmenuItemのindex
 */
uint8_t selectMenuPrompt(const char *message, const char *menuItems[], const int menuItemsLength);
/**
 * @brief ユーザーにパスワードを入力して貰うプロンプト
 * @note ユーザーに２回入力してもらう。
 * @return ハッシュ化されてないパスワード
 */
std::string inputPasswordPrompt();
/** 
 * @brief ユーザーがqキーを押すまで待機するプロンプト
 */
void pausePrompt();
} // namespace Prompt

/**
 * @file User.h
 * @brief ユーザーアカウントを保持するクラス
 */
#pragma once
#include "t_level.h"
#include <string>
/**
 * @brief ユーザーアカウントを保持するクラス
 * @attention このクラスではデータを保持するだけなので別途バリデーション等をすること
 */
class User {
  public:
    /** @brief 会員番号 */
    uint ID;
    /** @brief アカウント名 */
    std::string Name;
    /** @brief パスワード */
    std::string Pass;
    /** @brief アカウントの状態 (有効、無効) */
    bool Avail;
    /** @brief 権限 */
    t_Level Level;
    User(uint id,
         std::string name,
         std::string pass,
         bool avail,
         t_Level level);
    std::string toString();
};

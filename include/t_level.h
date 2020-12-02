/**
 * @file t_level.h
 * @brief ユーザーアカウントの権限を定義
 */
#pragma once
#include <string>
/**
 * @brief ユーザーアカウントの権限
 */
enum t_Level {
    ADMIN,
    PREM,
    GEN,
    TRY
};
/**
 * @brief 権限から文字列を生成
 */
std::string t_LevelToString(t_Level level);
/**
 * @brief 数字を権限へ変換
 * @attention 権限の数より大きな値が入力された場合 invalid_argument を出す
 */
t_Level intToT_Level(int level);

/**
 * @brief 文字列を権限へ変換
 * @attention 存在しない権限が入力された場合 invalid_argument を出す
 */
t_Level stringToT_Level(std::string level);

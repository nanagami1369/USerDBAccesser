/**
 * @file Controller.h
 * @brief UserDBをCUIで扱うためのクラス
 */
#pragma once
#include "UserDB.h"
#include <string>
/**
 * @brief UserDBをCUIから扱うコントローラー
 */
class Controller {
  private:
    UserDB *db;

    /**
     * @brief ユーザーIDを入力させるプロンプト
     */
    uint InputIdPrompt();
    /**
     * @brief ユーザーアカウントを追加する処理
     */
    void addUser();
    /**
     * @brief ユーザーアカウントを追加する処理
     */
    void removeUser();
    /**
     * @brief ユーザーアカウントの状態を変更する処理
     */
    void changeAvail();
    /**
     * @brief ユーザーアカウントを検索する処理
     */
    void searchUser();

    /**
     * @brief ユーザーアカウントをIdで検索する処理
     * @note searchUser関数から呼ばれる
     */
    void searchUserById();
    /**
     * @brief ユーザーアカウントを名前で検索する処理
     * @note searchUser関数から呼ばれる
     */
    void searchUserByName();
    /**
     * @brief ユーザーアカウントを状態で検索する処理
     * @note searchUser関数から呼ばれる
     */
    void searchUserByAvail();
    /**
     * @brief ユーザーアカウントを権限で検索する処理
     * @note searchUser関数から呼ばれる
     */
    void searchUserByLevel();

  public:
    Controller(UserDB *db);
    ~Controller();
    /**
     * @brief コントローラーを起動する。
     */
    void start();
};

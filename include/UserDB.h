#pragma once
#include "User.h"
#include "t_level.h"
#include <memory>
#include <string>
#include <vector>

class UserDB {
  private:
    /**
     * checkNameValidation関数でハッシュ化方式の指定に使用
     */
    static const char *SHA_512;
    /**
     * Id用バリデーションチェックの標準実装
     */
    virtual void checkIdValidation(const std::string id) final;
    /**
     * 名前用バリデーションチェックの標準実装
     */
    virtual void checkNameValidation(const std::string name) final;
    /**
     * パスワード用バリデーションチェックの標準実装
     */
    virtual void checkPassWordValidation(const std::string rowPass) final;
    /**
     * ハッシュ済みパスワードを作成する
     */
    virtual std::string createHashPassWord(const std::string rowPass, const char *HashMode) final;

  protected:
    /**
     * ユーザーの追加情報のバリデーション
     * add関数から呼ばれる
     */
    virtual void checkAddValidation(
        const std::string name,
        const std::string rowPass,
        const bool avail,
        const t_Level level);
    /**
     * 内部データベースへの追加に使用
     * add関数から呼ばれる
     */
    virtual void addInternalDatabase(
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) = 0;
    /**
     * ユーザーIDのバリデーション
     * search関数から呼ばれる
     */
    virtual void checkSearchValidation(const std::string id);
    /**
     * 内部データベースからの検索に使用
     * search関数から呼ばれる
     */
    virtual User searchInternalDatabase(const std::string id) = 0;
    /**
     * ユーザーIDのバリデーション
     * remove関数から呼ばれる
     */
    virtual void checkRemoveValidation(const std::string id);
    /**
     * 内部データベースからの削除に使用
     * remove関数から呼ばれる
     */
    virtual void removeInternalDatabase(const std::string id) = 0;
    /**
     * ユーザーの更新情報のバリデーション
     * update関数から呼ばれる
     * updateUserId:変更するアカウントのID(アカウントのIDは変更不可)
     * rowPass:値が空文字の場合変更なしと判断、それ以外の場合は変更
     *
     */
    virtual void checkUpdateValidation(
        const std::string updateUserId,
        const std::string name,
        const std::string rowPass,
        const bool avail,
        const t_Level level);
    /**
     * 内部データベースへの更新に使用
     * update関数から呼ばれる
     * updateUserId:変更するアカウントのID(アカウントのIDは変更不可)
     * hashedPass:値が空文字の場合変更なしと判断、それ以外の場合は変更すること
     */
    virtual void updateInternalDatabase(
        const std::string updateUserId,
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) = 0;

  public:
    /**
     * データベースへアカウントを追加
     */
    virtual void add(const std::string name,
                     const std::string rowPass,
                     const bool avail,
                     const t_Level level) final;
    /**
     * データベースからアカウントを検索
     */
    virtual User search(const std::string id) final;
    /**
     * データベースからアカウントを削除
     */
    virtual void remove(const std::string id) final;
    /**
     * データベースのアカウントを更新
     * updateUserId:変更するアカウントのID(アカウントのIDは変更不可)
     * rowPass:値が空文字の場合変更なしと判断、それ以外の場合は変更
     */
    virtual void update(
        const std::string updateUserId,
        const std::string name,
        const std::string rowPass,
        const bool avail,
        const t_Level level) final;
    /**
     * データベースのすべてのユーザー情報をコンソールへ出力
     */
    virtual void WriterAllUserToConsole() = 0;
    virtual ~UserDB() {}
};

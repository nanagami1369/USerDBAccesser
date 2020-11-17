/**
 * @file UserDB.h
 * @brief ユーザーDBの基本となる実装をまとめた抽象クラス
**/
#pragma once
#include "User.h"
#include "t_level.h"
#include <memory>
#include <string>
#include <vector>

class UserDB {
  private:
    /**
     * @brief checkNameValidation関数でハッシュ化方式の指定に使用
     */
    static const char *SHA_512;
    /**
     * @brief Id用バリデーションチェックの標準実装
     */
    virtual void checkIdValidation(const uint id) final;
    /**
     * @brief 名前用バリデーションチェックの標準実装
     */
    virtual void checkNameValidation(const std::string name) final;
    /**
     * @brief パスワード用バリデーションチェックの標準実装
     */
    virtual void checkPassWordValidation(const std::string rowPass) final;
    /**
     * @brief ハッシュ済みパスワードを作成する
     */
    virtual std::string createHashPassWord(const std::string rowPass, const char *HashMode) final;

  protected:
    /**
     * @brief ユーザーの追加情報のバリデーション
     * @note add関数から呼ばれる
     */
    virtual void checkAddValidation(
        const std::string name,
        const std::string rowPass,
        const bool avail,
        const t_Level level);
    /**
     * @brief 内部データベースへの追加に使用
     * @note add関数から呼ばれる
     */
    virtual void addInternalDatabase(
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) = 0;
    /**
     * @brief ユーザーIDのバリデーション
     * @note search関数から呼ばれる
     */
    virtual void checkSearchValidation(const uint id);
    /**
     * @brief 内部データベースからの検索に使用
     * @note search関数から呼ばれる
     */
    virtual User searchInternalDatabase(const uint id) = 0;
    /**
     * @brief ユーザーIDのバリデーション
     * @note remove関数から呼ばれる
     */
    virtual void checkRemoveValidation(const uint id);
    /**
     * @brief 内部データベースからの削除に使用
     * @note remove関数から呼ばれる
     */
    virtual void removeInternalDatabase(const uint id) = 0;
    /**
     * @brief ユーザーの更新情報のバリデーション
     * @note update関数から呼ばれる
     * @param updateUserId:変更するアカウントのID(アカウントのIDは変更不可)
     * @param rowPass:値が空文字の場合変更なしと判断、それ以外の場合は変更
     *
     */
    virtual void checkUpdateValidation(
        const uint updateUserId,
        const std::string name,
        const std::string rowPass,
        const bool avail,
        const t_Level level);
    /**
     * @brief 内部データベースへの更新に使用
     * @note update関数から呼ばれる
     * @param updateUserId:変更するアカウントのID(アカウントのIDは変更不可)
     * @param hashedPass:値が空文字の場合変更なしと判断、それ以外の場合は変更すること
     */
    virtual void updateInternalDatabase(
        const uint updateUserId,
        const std::string name,
        const std::string hashedPass,
        const bool avail,
        const t_Level level) = 0;

  public:
    /**
     * @brief データベースへアカウントを追加
     */
    virtual void add(const std::string name,
                     const std::string rowPass,
                     const bool avail,
                     const t_Level level) final;
    /**
     * @brief データベースからアカウントを検索
     */
    virtual User search(const uint id) final;
    /**
     * @brief データベースからアカウントを削除
     */
    virtual void remove(const uint id) final;
    /**
     * @brief データベースのアカウントを更新
     * @param updateUserId:変更するアカウントのID(アカウントのIDは変更不可)
     * @param rowPass:値が空文字の場合変更なしと判断、それ以外の場合は変更
     */
    virtual void update(
        const uint updateUserId,
        const std::string name,
        const std::string rowPass,
        const bool avail,
        const t_Level level) final;
    /**
     * @brief データベースのすべてのユーザー情報をコンソールへ出力
     */
    virtual void WriterAllUserToConsole() = 0;
    /**
     * @brief 最後に追加されたアカウントのIdを返却する
     * @return 最後に追加されたのId
     */
    virtual uint GetLastId() = 0;
    virtual ~UserDB() {}
};

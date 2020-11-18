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
     * @note searchById関数から呼ばれる
     */
    virtual void checkSearchByIdValidation(const uint id);
    /**
     * @brief 内部データベースからの検索に使用
     * @note searchById関数から呼ばれる
     */
    virtual User searchByIdInternalDatabase(const uint id) = 0;
    /**
     * @brief ユーザー名のバリデーション
     * @note searchByName関数から呼ばれる
     */
    virtual void checkSearchByNameValidation(const std::string name) final;
    /**
     * @brief 内部データベースからの検索に使用
     * @note searchByName関数から呼ばれる
     */
    virtual std::vector<User> searchByNameInternalDatabase(const std::string name) = 0;
    /**
     * @brief 内部データベースからの検索に使用
     * @note searchByAvail関数から呼ばれる
     */
    virtual std::vector<User> searchByAvailInternalDatabase(const bool avail) = 0;
    /**
     * @brief 内部データベースからの検索に使用
     * @note searchByLevel関数から呼ばれる
     */
    virtual std::vector<User> searchByLevelInternalDatabase(const t_Level level) = 0;
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
     * @brief データベースからIdでアカウントを検索
     */
    virtual User searchById(const uint id) final;
    /**
     * @brief データベースから名前でアカウントを検索
     * @note 部分一致で検索
     */
    virtual std::vector<User> searchByName(const std::string name) final;
    /**
     * @brief データベースからアカウントが有効か、無効かでアカウントを検索
     */
    virtual std::vector<User> searchByAvail(const bool avail) final;
    /**
     * @brief データベースからレベルで検索
     */
    virtual std::vector<User> searchByLevel(const t_Level level) final;
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

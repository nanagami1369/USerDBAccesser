#include "CGIController.h"
#include <sstream>
#include <stdexcept>
CGIController::CGIController(UserDB *db) {
    this->db = db;
}

CGIController::~CGIController() {
    delete db;
}
static void sendOK200Json(const nlohmann::json &json);
static void sendOK200TextMessage(const std::string &message);
static void sendCreated201TextMessage(const std::string &message);
static void sendOk200DELETETextMessage(const std::string message);
static void sendBadRequest400TextMessage(const std::string &errorMessage);

std::string CGIController::getPostData() {
    //環境変数から送られたデータサイズを取得する
    char *str = std::getenv("CONTENT_LENGTH");
    if (str == NULL) {
        throw std::invalid_argument("必須パラメータが設定されてません");
    }
    int length = atol(str) + 1;
    //もし、サイズが大きすぎるときはエラー
    if (length > 1000000)
        throw std::invalid_argument("データが大きすぎます");
    std::string value;
    int character;
    auto input = new char[length];
    int index = 0;
    while ((character = std::getchar()) != EOF) {
        input[index] = character;
        index++;
    }
    input[length - 1] = '\0';
    return std::string(input);
}

void CGIController::addUser(const nlohmann::json &addUserJson) {
    std::string name;
    std::string rowPass;
    bool avail;
    t_Level level;
    try {
        name = addUserJson["name"].get<std::string>();
        if (name.empty()) {
            sendBadRequest400TextMessage("名前に何も入力されてません 入力データ：{" + addUserJson.dump() + "}");
            return;
        }
        rowPass = addUserJson["pass"].get<std::string>();
        if (rowPass.empty()) {
            sendBadRequest400TextMessage("パスワードに何も入力されてません 入力データ：{" + addUserJson.dump() + "}");
            return;
        }
        if (rowPass.length() < 8) {
            sendBadRequest400TextMessage("パスワードは8文字以上にして下さい");
            return;
        }
        avail = addUserJson["avail"].get<std::string>() == "true" ? true : false;
        auto stringlevel = addUserJson["level"].get<std::string>();
        level = stringToT_Level(stringlevel);
    } catch (const nlohmann::json::type_error &e) {
        sendBadRequest400TextMessage("必須パラメータが設定されてません 入力データ：{" + addUserJson.dump() + "}");
        return;
    } catch (const std::invalid_argument &e) {
        sendBadRequest400TextMessage("存在しない権限が指定されました");
        return;
    }
    db->add(name, rowPass, avail, level);
    sendCreated201TextMessage("アカウントが追加されました");
}

void CGIController::removeUsers(const nlohmann::json &removeUserJson) {
    auto ids = removeUserJson["ids"].get<std::vector<uint>>();
    if (ids.empty()) {
        sendBadRequest400TextMessage("アカウントが存在しませんでした");
        return;
    }

    // 存在しないアカウントがあるかチェック
    std::string message;
    for (auto &id : ids) {
        try {
            db->searchById(id);
        } catch (const std::range_error &e) {
            message.append("ID - ");
            message.append(std::to_string(id));
            message.append(" アカウントが存在しませんでした\n");
        }
    }
    if (!message.empty()) {
        sendBadRequest400TextMessage(message);
        return;
    }
    // 削除処理
    for (auto &id : ids) {
        try {
            db->remove(id);
        } catch (const std::range_error &e) {
            sendBadRequest400TextMessage(e.what());
            return;
        }
    }
    sendOk200DELETETextMessage("選択された全てのアカウントを削除しました" + message);
    return;
}

void CGIController::searchUsers(const nlohmann::json &searchUserDataJson) {
    auto name = searchUserDataJson["name"].get<std::string>();
    auto avail = searchUserDataJson["avail"].get<std::string>();
    auto level = searchUserDataJson["level"].get<std::string>();
    // 一番最初に指定された条件でDBに検索をかける。
    // ２つ目以降の条件はクライアント側で処理をする。
    // 何も条件を指定されなかった場合は0件返す
    std::vector<nlohmann::json> usersJson;

    if (name.length() != 0) {
        try {
            // 名前で検索
            auto users = this->db->searchByName(name);
            for (auto &user : users) {
                nlohmann::json userJson = {
                    {"id", user.ID},
                    {"name", user.Name},
                    {"pass", "********"},
                    {"avail", std::to_string(user.Avail)},
                    {"level", t_LevelToString(user.Level)}};
                usersJson.push_back(userJson);
            }
        } catch (const std::range_error &e) {
            // 検索数が0件なら0件で返す
        }
    } else if (avail != "null") {
        // 状態で検索
        try {
            auto users = this->db->searchByAvail(avail == "true" ? true : false);
            for (auto &user : users) {
                nlohmann::json userJson = {
                    {"id", user.ID},
                    {"name", user.Name},
                    {"pass", "********"},
                    {"avail", std::to_string(user.Avail)},
                    {"level", t_LevelToString(user.Level)}};
                usersJson.push_back(userJson);
            }
        } catch (const std::range_error &e) {
            // 検索数が0件なら0件で返す
        }
    } else if (level != "null") {
        // 権限で検索
        try {
            auto users = this->db->searchByLevel(stringToT_Level(level));
            for (auto &user : users) {
                nlohmann::json userJson = {
                    {"id", user.ID},
                    {"name", user.Name},
                    {"pass", "********"},
                    {"avail", std::to_string(user.Avail)},
                    {"level", t_LevelToString(user.Level)}};
                usersJson.push_back(userJson);
            }
        } catch (const std::range_error &e) {
            // 検索数が0件なら0件で返す
        }
    } else {
        // 検索条件が設定されていない場合0件で返す
    }
    nlohmann::json searchUsersJson = {
        {"users", usersJson}};
    sendOK200Json(searchUsersJson);
    return;
}

void CGIController::updateUser(const nlohmann::json &updateUserJson) {
    uint id;
    std::string name;
    std::string rowPass;
    bool avail;
    t_Level level;
    id = updateUserJson["id"].get<uint>();
    name = updateUserJson["name"].get<std::string>();
    if (name.empty()) {
        sendBadRequest400TextMessage("名前に何も入力されてません 入力データ：{" + updateUserJson.dump() + "}");
        return;
    }
    rowPass = updateUserJson["pass"].get<std::string>();
    if (rowPass.length() != 0 && rowPass.length() < 8) {
        sendBadRequest400TextMessage("パスワードは8文字以上にして下さい");
        return;
    }
    avail = updateUserJson["avail"].get<std::string>() == "true" ? true : false;
    auto stringlevel = updateUserJson["level"].get<std::string>();
    try {
        level = stringToT_Level(stringlevel);
    } catch (const std::invalid_argument &e) {
        sendBadRequest400TextMessage("存在しない権限が指定されました");
        return;
    }
    db->update(id, name, rowPass, avail, level);
    sendOK200TextMessage("アカウントが更新されました");
    return;
}

void CGIController::getAllUser() {
    auto allUsers = db->GetAllUserData();
    std::vector<nlohmann::json> usersJson;
    for (auto &user : allUsers) {
        nlohmann::json userJson = {
            {"id", user.ID},
            {"name", user.Name},
            {"pass", "********"},
            {"avail", std::to_string(user.Avail)},
            {"level", t_LevelToString(user.Level)}};
        usersJson.push_back(userJson);
    }

    nlohmann::json allUsersJson = {
        {"users", usersJson}};
    sendOK200Json(allUsersJson);
}

void CGIController::execMethod(const std::string &methodName, const nlohmann::json &args) {
    if (methodName == "add") {
        addUser(args);
        return;
    } else if (methodName == "remove") {
        removeUsers(args);
        return;
    } else if (methodName == "search") {
        searchUsers(args);
    } else if (methodName == "update") {
        updateUser(args);
    } else if (methodName == "getAll") {
        getAllUser();
        return;
    }
}

static void sendBadRequest400TextMessage(const std::string &errorMessage) {
    auto header = "Content-type: text/plain; charset=utf-8\n"
                  "Status: 400 Bad Request\n"
                  "Pragma: no-cache\n"
                  "Cache-Control: no-cache\n\n";
    std::printf("%s", header);
    std::printf("%s", errorMessage.c_str());
}

static void sendOK200Json(const nlohmann::json &json) {
    auto header = "Content-type: application/json; charset=utf-8\n"
                  "Status: 200 OK\n"
                  "Access-Control-Allow-Methods: POST\n"
                  "Pragma: no-cache\n"
                  "Cache-Control: no-cache\n\n";
    std::printf("%s", header);
    std::printf("%s", json.dump().c_str());
}

static void sendOK200TextMessage(const std::string &message) {
    auto header = "Content-type: text/plain; charset=utf-8\n"
                  "Status: 200 OK\n"
                  "Access-Control-Allow-Methods: POST\n"
                  "Pragma: no-cache\n"
                  "Cache-Control: no-cache\n\n";
    std::printf("%s", header);
    std::printf("%s", message.c_str());
}

static void sendCreated201TextMessage(const std::string &message) {
    auto header = "Content-type: text/plain; charset=utf-8\n"
                  "Status: 201 Created\n"
                  "Access-Control-Allow-Methods: POST\n"
                  "Pragma: no-cache\n"
                  "Cache-Control: no-cache\n\n";
    std::printf("%s", header);
    std::printf("%s", message.c_str());
}

static void sendOk200DELETETextMessage(const std::string message) {
    auto header = "Content-type: text/plain; charset=utf-8\n"
                  "Status: 200 OK\n"
                  "Access-Control-Allow-Methods: DELETE\n"
                  "Pragma: no-cache\n"
                  "Cache-Control: no-cache\n\n";
    std::printf("%s", header);
    std::printf("%s", message.c_str());
}

void CGIController::exec() {
    // POSTデータの取得
    std::string inputData;
    try {
        inputData = getPostData();
    } catch (const std::invalid_argument &e) {
        sendBadRequest400TextMessage(e.what());
        return;
    }
    // Jsonに変換
    nlohmann::json inputJson;
    try {
        inputJson = nlohmann::json::parse(inputData);
    } catch (const nlohmann::json::parse_error &e) {
        std::string errMessage = "jsonのパースに失敗しました 入力データ:{" + inputData + "}";
        sendBadRequest400TextMessage(errMessage);
        return;
    }
    // リクエストの目的を確認
    std::string methodName;
    try {
        methodName = inputJson["method"].get<std::string>();
    } catch (const nlohmann::json::type_error &e) {
        std::string errMessage = "必須パラメータが設定されてません 入力データ{" + inputJson.dump() + "}";
        sendBadRequest400TextMessage(errMessage);
        return;
    }
    // 各種DB操作
    execMethod(methodName, inputJson);
    return;
}

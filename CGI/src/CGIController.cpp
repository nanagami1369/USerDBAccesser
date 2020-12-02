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
static void sendBadRequest400JsonMessage(const std::string &errorMessage);

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
    } else if (methodName == "getAll") {
        getAllUser();
        return;
    }
}

static void sendBadRequest400JsonMessage(const std::string &errorMessage) {
    auto header = "Content-type: application/json; charset=utf-8\n"
                  "Status: 400 Bad Request\n"
                  "Pragma: no-cache\n"
                  "Cache-Control: no-cache\n\n";
    nlohmann::json errjson = {
        {"message", errorMessage},
    };
    std::printf("%s", header);
    std::printf("%s", errjson.dump().c_str());
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

void CGIController::exec() {
    // POSTデータの取得
    std::string inputData;
    try {
        inputData = getPostData();
    } catch (const std::invalid_argument &e) {
        sendBadRequest400JsonMessage(e.what());
        return;
    }
    // Jsonに変換
    nlohmann::json inputJson;
    try {
        inputJson = nlohmann::json::parse(inputData);
    } catch (const nlohmann::json::parse_error &e) {
        std::string errMessage = "jsonのパースに失敗しました 入力データ:{" + inputData + "}";
        sendBadRequest400JsonMessage(errMessage);
        return;
    }
    // リクエストの目的を確認
    std::string methodName;
    try {
        methodName = inputJson["method"].get<std::string>();
    } catch (const nlohmann::json::type_error &e) {
        std::string errMessage = "必須パラメータが設定されてません 入力データ{" + inputJson.dump() + "}";
        sendBadRequest400JsonMessage(errMessage);
        return;
    }
    // 各種DB操作
    execMethod(methodName, inputJson);
    return;
}

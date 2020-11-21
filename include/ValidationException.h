/**
 * @file ValidationException.h
 * @brief バリデーションエラーを管理するクラス
 */
#include <stdexcept>
/**
 * @brief バリデーションエラーを管理するクラス
 */
class ValidationException : public std::runtime_error {
  public:
    ValidationException(const char *_Message);
};

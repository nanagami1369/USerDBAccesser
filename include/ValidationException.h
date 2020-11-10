#include <stdexcept>
class ValidationException : public std::runtime_error {
  public:
    ValidationException(const char *_Message);
};

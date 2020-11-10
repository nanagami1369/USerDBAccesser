#include "ValidationException.h"
ValidationException::ValidationException(const char *_Message) : runtime_error(_Message) {}

#include <cstring>
#include "fileerror.hh"

using namespace yapet;

FileError::FileError(const char* msg, int errorNumber)
    : std::runtime_error{msg},
      _errorNumber{errorNumber},
      _systemErrorMsg{nullptr} {
    if (_errorNumber > NO_SYSTEM_ERROR_SPECIFIED) {
        _systemErrorMsg = std::strerror(_errorNumber);
    }
}
#ifndef _FILEERROR_HH
#define _FILEERROR_HH

#include <stdexcept>

namespace yapet {
constexpr auto NO_SYSTEM_ERROR_SPECIFIED = -1;

class FileError : public std::runtime_error {
   private:
    int _errorNumber;
    const char* _systemErrorMsg;

   public:
    FileError(const char* msg, int errorNumber = NO_SYSTEM_ERROR_SPECIFIED);

    int errorNumber() const { return _errorNumber; }
    const char* systemErrorMsg() const { return _systemErrorMsg; }
};

class FileInsecureError : public FileError {
   public:
    FileInsecureError(const char* msg)
        : FileError{msg, NO_SYSTEM_ERROR_SPECIFIED} {}
};

class FileFormatError : public FileError {
   public:
    FileFormatError(const char* msg)
        : FileError{msg, NO_SYSTEM_ERROR_SPECIFIED} {}
};
}  // namespace yapet

#endif
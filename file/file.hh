#ifndef _FILE_HH
#define _FILE_HH

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "securearray.hh"

namespace yapet {
class File {
   private:
    std::string _filename;
    FILE* _file;
    bool _openFlag;

   public:
    File(const std::string& filename) noexcept;
    ~File();

    File(const File&) = delete;
    File& operator=(const File&) = delete;

    File(File&&);
    File& operator=(File&&);

    void openExisting();
    void openNew();

    bool isOpen() const { return _openFlag; }

    /**
     * Read \c size bytes.
     *
     * The bytes read are returned in a \c SecureArray.
     */
    std::pair<SecureArray, bool> read(std::uint32_t size);
    /**
     * Read the next record.
     *
     * Read the length indicator and return a \c SecureArray holding the data.
     */
    std::pair<SecureArray, bool> read();

    /**
     * Write \c SecureArray to file.
     *
     * Write the content of the \c SecureArray preceded by the size of the
     * record.
     */
    void write(const SecureArray& secureArray);
    /**
     * Write to file
     */
    void write(const std::uint8_t* buffer, std::uint32_t size);

    std::string filename() const { return _filename; }

    void rewind();
};

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
}  // namespace yapet

#endif
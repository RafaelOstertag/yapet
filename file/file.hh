#ifndef _FILE_HH
#define _FILE_HH

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>

#include "securearray.hh"

namespace yapet {
class File {
   private:
    std::string _filename;
    std::basic_fstream<std::uint8_t> stream;

   public:
    File(const std::string& filename);
    ~File();

    File(const File&) = delete;
    File& operator=(const File&) = delete;

    File(File&&);
    File& operator=(File&&);

    /**
     * Read \c size bytes.
     *
     * The bytes read are returned in a \c SecureArray.
     */
    SecureArray read(int size);
    /**
     * Read the next record.
     *
     * Read the length indicator and return a \c SecureArray holding the data.
     */
    SecureArray read();

    std::string filename() { return _filename; }
};

class FileError : public std::runtime_error {
   public:
    FileError(const char* msg) : std::runtime_error{msg} {}
};
}  // namespace yapet

#endif
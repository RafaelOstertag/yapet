#ifndef _RAWFILE_HH
#define _RAWFILE_HH

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <utility>

#include "securearray.hh"

namespace yapet {
class RawFile {
   private:
    std::string _filename;
    FILE* _file;
    bool _openFlag;

   public:
    RawFile(const std::string& filename) noexcept;
    ~RawFile();

    RawFile(const RawFile&) = delete;
    RawFile& operator=(const RawFile&) = delete;

    RawFile(RawFile&&);
    RawFile& operator=(RawFile&&);

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
}  // namespace yapet

#endif
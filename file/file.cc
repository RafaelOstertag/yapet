#include <stdexcept>

#include "file.hh"
#include "intl.h"

using namespace yapet;

constexpr const char* UNEXPECTED_EOF_MSG{
    "Unexpected end of file encountered while reading file"};
constexpr const char* FILE_READ_ERROR_MSG{"Error while reading file"};

File::File(const std::string& filename)
    : _filename{filename}, stream{filename} {}

File::~File() {}

File::File(File&& other)
    : _filename{std::move(other._filename)}, stream{std::move(other.stream)} {}

File& File::operator=(File&& other) {
    if (this == &other) {
        return *this;
    }

    _filename = std::move(_filename);
    stream = std::move(other.stream);
    return *this;
}

SecureArray File::read(int size) {
    if (size < 1) {
        throw std::invalid_argument(_("Read size must not be less than 1"));
    }

    SecureArray array(size);

    stream.read(*array, size);

    if (stream.eof()) {
        throw FileError(_(UNEXPECTED_EOF_MSG));
    }

    if (stream.bad() || stream.fail()) {
        throw FileError(_(FILE_READ_ERROR_MSG));
    }

    return std::move(array);
}

SecureArray File::read() {
    std::uint32_t odsRecordSize;

    stream >> odsRecordSize;
    if (stream.eof()) {
        throw FileError()
    }

    std::uint32_t
}

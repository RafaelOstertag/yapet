#include <cerrno>
#include <cstring>
#include <stdexcept>

#include "file.hh"
#include "intl.h"
#include "ods.hh"

using namespace yapet;

const auto FILE_NOT_OPEN{_("File not open")};
const auto FILE_ALREADY_OPEN{_("File already open")};
constexpr auto READ_WRITE_EXISTING_MODE{"r+"};
constexpr auto CREATE_NEW_MODE{"w+"};
constexpr auto ONE_ITEM{1};

inline void throwIfFileAlreadyOpen(bool alreadyOpen) {
    if (alreadyOpen) {
        throw FileError{FILE_ALREADY_OPEN};
    }
}

inline void throwIfFileNotOpen(bool fileOpen) {
    if (!fileOpen) {
        throw FileError{FILE_NOT_OPEN};
    }
}

File::File(const std::string& filename) noexcept
    : _filename{filename}, _file{nullptr}, _openFlag{false} {}

File::~File() {
    if (_file) {
        std::fclose(_file);
    }
}

File::File(File&& other)
    : _filename{std::move(other._filename)},
      _file{other._file},
      _openFlag{other._openFlag} {
    other._file = nullptr;
    other._openFlag = false;
}

File& File::operator=(File&& other) {
    if (this == &other) {
        return *this;
    }

    _filename = std::move(other._filename);
    _file = other._file;
    other._file = nullptr;
    _openFlag = other._openFlag;
    other._openFlag = false;

    return *this;
}

void File::openExisting() {
    throwIfFileAlreadyOpen(_openFlag);

    _file = std::fopen(_filename.c_str(), READ_WRITE_EXISTING_MODE);
    if (_file == nullptr) {
        throw FileError{_("Cannot open existing file"), errno};
    }
    _openFlag = true;
}

void File::openNew() {
    throwIfFileAlreadyOpen(_openFlag);

    _file = std::fopen(_filename.c_str(), CREATE_NEW_MODE);
    if (_file == nullptr) {
        throw FileError{_("Cannot create new file"), errno};
    }
    _openFlag = true;
}

std::pair<SecureArray, bool> File::read(std::uint32_t size) {
    throwIfFileNotOpen(_openFlag);

    if (size < 1) {
        throw FileError(_("Read size must not be less than 1"));
    }

    SecureArray array(size);

    auto res = std::fread(*array, size, ONE_ITEM, _file);
    if (std::feof(_file)) {
        return std::pair<SecureArray, bool>{SecureArray{1}, false};
    }
    if (std::ferror(_file) || res != ONE_ITEM) {
        throw FileError{_("Error reading file"), errno};
    }

    return std::pair<SecureArray, bool>{array, true};
}

std::pair<SecureArray, bool> File::read() {
    throwIfFileNotOpen(_openFlag);

    record_size_type odsRecordSize;

    auto res =
        std::fread(&odsRecordSize, sizeof(record_size_type), ONE_ITEM, _file);
    if (res != ONE_ITEM || std::feof(_file)) {
        return std::pair<SecureArray, bool>{SecureArray{1}, false};
    }

    if (std::ferror(_file)) {
        throw FileError{_("Error reading record length from file"), errno};
    }

    auto hostRecordSize = toHost(odsRecordSize);

    return read(hostRecordSize);
}

void File::write(const SecureArray& secureArray) {
    throwIfFileNotOpen(_openFlag);

    record_size_type hostRecordSize = secureArray.size();
    auto odsRecordSize = toODS(hostRecordSize);

    auto res =
        std::fwrite(&odsRecordSize, sizeof(record_size_type), ONE_ITEM, _file);
    if (res != ONE_ITEM || std::feof(_file) || std::ferror(_file)) {
        throw FileError{_("Cannot write record size to file"), errno};
    }

    write(*secureArray, hostRecordSize);
}

void File::write(const std::uint8_t* buffer, std::uint32_t size) {
    throwIfFileNotOpen(_openFlag);

    auto res = std::fwrite(buffer, size, ONE_ITEM, _file);
    if (res != ONE_ITEM || std::feof(_file) || std::ferror(_file)) {
        throw FileError{_("Cannot write buffer to file"), errno};
    }
}

void File::rewind() {
    throwIfFileNotOpen(_openFlag);

    auto error = std::fseek(_file, 0, SEEK_SET);
    if (error || std::feof(_file) || std::ferror(_file)) {
        throw FileError{_("Error rewinding file"), errno};
    }
}

FileError::FileError(const char* msg, int errorNumber)
    : std::runtime_error{msg},
      _errorNumber{errorNumber},
      _systemErrorMsg{nullptr} {
    if (_errorNumber > NO_SYSTEM_ERROR_SPECIFIED) {
        _systemErrorMsg = std::strerror(_errorNumber);
    }
}
#include <cerrno>
#include <stdexcept>

#include "fileerror.hh"
#include "intl.h"
#include "ods.hh"
#include "rawfile.hh"

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

RawFile::RawFile(const std::string& filename) noexcept
    : _filename{filename}, _file{nullptr}, _openFlag{false} {}

RawFile::~RawFile() { close(); }

RawFile::RawFile(RawFile&& other)
    : _filename{std::move(other._filename)},
      _file{other._file},
      _openFlag{other._openFlag} {
    other._file = nullptr;
    other._openFlag = false;
}

RawFile& RawFile::operator=(RawFile&& other) {
    if (this == &other) {
        return *this;
    }

    close();

    _filename = std::move(other._filename);

    _file = other._file;
    other._file = nullptr;

    _openFlag = other._openFlag;
    other._openFlag = false;

    return *this;
}

void RawFile::openExisting() {
    throwIfFileAlreadyOpen(_openFlag);

    _file = std::fopen(_filename.c_str(), READ_WRITE_EXISTING_MODE);
    if (_file == nullptr) {
        throw FileError{_("Cannot open existing file"), errno};
    }
    _openFlag = true;
}

void RawFile::openNew() {
    throwIfFileAlreadyOpen(_openFlag);

    _file = std::fopen(_filename.c_str(), CREATE_NEW_MODE);
    if (_file == nullptr) {
        throw FileError{_("Cannot create new file"), errno};
    }
    _openFlag = true;
}

std::pair<SecureArray, bool> RawFile::read(std::uint32_t size) {
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

std::pair<SecureArray, bool> RawFile::read() {
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

void RawFile::write(const SecureArray& secureArray) {
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

void RawFile::write(const std::uint8_t* buffer, std::uint32_t size) {
    throwIfFileNotOpen(_openFlag);

    auto res = std::fwrite(buffer, size, ONE_ITEM, _file);
    if (res != ONE_ITEM || std::feof(_file) || std::ferror(_file)) {
        throw FileError{_("Cannot write buffer to file"), errno};
    }
}

void RawFile::rewind() { seekAbsolute(0); }

void RawFile::seekAbsolute(seek_type position) {
    throwIfFileNotOpen(_openFlag);

    if (position < 0) {
        throw std::invalid_argument{_("Position must be positive")};
    }

    auto error = std::fseek(_file, position, SEEK_SET);
    if (error || std::feof(_file) || std::ferror(_file)) {
        throw FileError{_("Error seeking in file"), errno};
    }
}

void RawFile::close() {
    if (_file) {
        std::fclose(_file);
        _file = nullptr;
    }
    _openFlag = false;
}

void RawFile::reopen() {
    throwIfFileNotOpen(_openFlag);

    _file = ::freopen(_filename.c_str(), READ_WRITE_EXISTING_MODE, _file);
    if (_file == nullptr) {
        throw FileError(_("Error re-opening file"), errno);
    }
}

RawFile::seek_type RawFile::getPosition() {
    auto currentPosition = ::ftell(_file);
    if (currentPosition == -1) {
        throw FileError(_("Unable to get current position in file"), errno);
    }

    return currentPosition;
}
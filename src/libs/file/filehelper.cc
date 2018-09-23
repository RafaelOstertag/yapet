#include <cstdio>

#include "consts.h"
#include "fileerror.hh"
#include "filehelper.hh"
#include "yapet10file.hh"
#include "yapet20file.hh"

using namespace yapet;

SecureArray yapet::readFileIdentifier(const std::string& filename) {
    try {
        Yapet10File file{filename, false, false};
        file.open();
        return file.readIdentifier();
    } catch (...) {
        // try next file type
    }

    try {
        Yapet20File file{filename, false, false};
        file.open();
        return file.readIdentifier();
    } catch (...) {
        // handled below
    }

    char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
    std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                  _("'%s' is not a known file type"), filename.c_str());
    throw FileFormatError(msg);
}

bool yapet::isFileType(const SecureArray& expected,
                       const std::string& filename) {
    auto identifier{readFileIdentifier(filename)};
    return identifier == expected;
}

std::shared_ptr<YapetFile> yapet::getFile(const std::string& filename,
                                          bool secure) {
    SecureArray yapet10Identifier{toSecureArray(
        Yapet10File::RECOGNITION_STRING, Yapet10File::RECOGNITION_STRING_SIZE)};
    if (isFileType(yapet10Identifier, filename)) {
        return std::shared_ptr<YapetFile>{
            new Yapet10File{filename, false, secure}};
    }

    SecureArray yapet20Identifier{toSecureArray(
        Yapet20File::RECOGNITION_STRING, Yapet20File::RECOGNITION_STRING_SIZE)};
    if (isFileType(yapet20Identifier, filename)) {
        return std::shared_ptr<YapetFile>{
            new Yapet20File{filename, false, secure}};
    }

    char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
    std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                  _("'%s' is not a known file type"), filename.c_str());
    throw FileFormatError(msg);
}

MetaData yapet::readMetaData(const std::string& filename, bool secure) {
    auto yapetFile{getFile(filename, secure)};
    yapetFile->open();
    return yapetFile->readUnencryptedMetaData();
}
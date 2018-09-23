#include "cryptofactoryhelper.hh"
#include "aes256factory.hh"
#include "blowfishfactory.hh"
#include "fileerror.hh"
#include "filehelper.hh"
#include "intl.h"

using namespace yapet;

namespace {
bool isYapet10File(const std::string& filename) {
    SecureArray expectedIdentifier{toSecureArray(
        Yapet10File::RECOGNITION_STRING, Yapet10File::RECOGNITION_STRING_SIZE)};

    return isFileType(expectedIdentifier, filename);
}

bool isYapet20File(const std::string& filename) {
    SecureArray expectedIdentifier{toSecureArray(
        Yapet20File::RECOGNITION_STRING, Yapet20File::RECOGNITION_STRING_SIZE)};

    return isFileType(expectedIdentifier, filename);
}
}  // namespace

std::shared_ptr<AbstractCryptoFactory> yapet::getCryptoFactoryForFile(
    const std::string& filename, const SecureArray& password) {
    try {
        if (isYapet10File(filename)) {
            return std::shared_ptr<AbstractCryptoFactory>{
                new BlowfishFactory{password, MetaData{}}};
        }
    } catch (std::exception&) {
        // Ok, not a Yapet file
    }

    try {
        if (isYapet20File(filename)) {
            auto metaData{readMetaData(filename, false)};
            return std::shared_ptr<AbstractCryptoFactory>{
                new Aes256Factory{password, metaData}};
        }
    } catch (std::exception&) {
        // Ok, not a Yapet file
    }

    return std::shared_ptr<AbstractCryptoFactory>{};
}
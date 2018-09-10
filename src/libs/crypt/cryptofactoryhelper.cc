#include "cryptofactoryhelper.hh"
#include "aes256factory.hh"
#include "blowfishfactory.hh"
#include "fileerror.hh"
#include "rawfile.hh"

using namespace yapet;

namespace {
bool isYapet10File(const std::string& filename) {
    RawFile file{filename};
    file.openExisting();

    auto actualIdentifierPair{file.read(Yapet10File::RECOGNITION_STRING_SIZE)};
    if (actualIdentifierPair.second == false) {
        throw FileFormatError{_("Cannot read file identifier from file")};
    }

    SecureArray expectedIdentifier{toSecureArray(
        Yapet10File::RECOGNITION_STRING, Yapet10File::RECOGNITION_STRING_SIZE)};

    return actualIdentifierPair.first == expectedIdentifier;
}

bool isYapet20File(const std::string& filename) {
    RawFile file{filename};
    file.openExisting();

    auto actualIdentifierPair{file.read(Yapet20File::RECOGNITION_STRING_SIZE)};
    if (actualIdentifierPair.second == false) {
        throw FileFormatError{_("Cannot read file identifier from file")};
    }

    SecureArray expectedIdentifier{toSecureArray(
        Yapet20File::RECOGNITION_STRING, Yapet20File::RECOGNITION_STRING_SIZE)};

    return actualIdentifierPair.first == expectedIdentifier;
}
}  // namespace

std::shared_ptr<AbstractCryptoFactory> yapet::getCryptoFactoryForFile(
    const std::string& filename, const SecureArray& password) {
    try {
        if (isYapet10File(filename)) {
            return std::shared_ptr<AbstractCryptoFactory>{
                new BlowfishFactory{password}};
        }
    } catch (std::exception) {
        // Ok, not a Yapet file
    }

    try {
        if (isYapet20File(filename)) {
            return std::shared_ptr<AbstractCryptoFactory>{
                new Aes256Factory{password}};
        }
    } catch (std::exception) {
        // Ok, not a Yapet file
    }

    return std::shared_ptr<AbstractCryptoFactory>{};
}
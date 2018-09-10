#ifndef _CRYPTOFACTORYHELPER_HH
#define _CRYPTOFACTORYHELPER_HH

#include "abstractcryptofactory.hh"

namespace yapet {
std::shared_ptr<AbstractCryptoFactory> getCryptoFactoryForFile(
    const std::string& filename, const SecureArray& password);
}

#endif
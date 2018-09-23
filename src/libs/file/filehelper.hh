#ifndef _FILEHELPER_H
#define _FILEHELPER_H

#include <memory>

#include "metadata.hh"
#include "securearray.hh"
#include "yapetfile.hh"

namespace yapet {
SecureArray readFileIdentifier(const std::string& filename);
bool isFileType(const SecureArray& expected, const std::string& filename);
std::shared_ptr<YapetFile> getFile(const std::string& filename, bool secure);
MetaData readMetaData(const std::string& filename, bool secure);
}  // namespace yapet

#endif
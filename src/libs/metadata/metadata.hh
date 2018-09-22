#ifndef _METADATA_HH
#define _METADATA_HH

#include <unordered_map>

#include "serializable.hh"

namespace yapet {
class MetaData : public Serializable {
   private:
    std::unordered_map<std::string, int> _dataItems;

   public:
    MetaData();
    MetaData(const SecureArray& serialized);
    MetaData(const MetaData& other);
    MetaData(MetaData&& other);
    MetaData& operator=(const MetaData& other);
    MetaData& operator=(MetaData&& other);

    int getValue(const std::string& key) const;
    void setValue(const std::string& key, int value);

    std::unordered_map<std::string, int>::size_type size() const;

    SecureArray serialize() const;
};
}  // namespace yapet

#endif
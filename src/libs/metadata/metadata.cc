#include <cstdint>
#include <stdexcept>
#include <vector>

#include "consts.h"
#include "metadata.hh"
#include "ods.hh"

using namespace yapet;

MetaData::MetaData() : _dataItems{} {}

namespace {
std::string readKey(const SecureArray& serialized,
                    SecureArray::size_type& pos) {
    std::string str{};
    for (; serialized[pos] != '\0'; pos++) {
        str += static_cast<char>(serialized[pos]);
    }
    // advance to int position
    pos++;

    return str;
}

int readValue(const SecureArray& serialized, SecureArray::size_type& pos) {
    SecureArray::size_type end = pos + sizeof(int);

    int odsInt;
    std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(&odsInt);

    for (SecureArray::size_type i = 0; pos < end; i++, pos++) {
        ptr[i] = serialized[pos];
    }

    return toHost(odsInt);
}
}  // namespace

MetaData::MetaData(const SecureArray& serialized) : _dataItems{} {
    if (serialized == SecureArray{}) {
        return;
    }

    try {
        for (SecureArray::size_type positionInSerialized = 0;
             positionInSerialized < serialized.size();) {
            std::string key = readKey(serialized, positionInSerialized);

            int value = readValue(serialized, positionInSerialized);
            _dataItems[key] = value;
        }
    } catch (...) {
        throw std::invalid_argument(_("Error deserializing meta data"));
    }
}

MetaData::MetaData(const MetaData& other) : _dataItems{other._dataItems} {}

MetaData::MetaData(MetaData&& other) : _dataItems{move(other._dataItems)} {}

MetaData& MetaData::operator=(const MetaData& other) {
    if (this == &other) return *this;

    _dataItems = other._dataItems;

    return *this;
}

MetaData& MetaData::operator=(MetaData&& other) {
    if (this == &other) return *this;

    _dataItems = move(other._dataItems);

    return *this;
}

int MetaData::getValue(const std::string& key) const {
    return _dataItems.at(key);
}

void MetaData::setValue(const std::string& key, int value) {
    _dataItems[key] = value;
}

namespace {
void addStringToBuffer(std::vector<std::uint8_t>& buffer,
                       const std::string& str) {
    for (char c : str) {
        buffer.push_back(static_cast<std::uint8_t>(c));
    }
    buffer.push_back(0);
}

void addIntToBuffer(std::vector<std::uint8_t>& buffer, const int& i) {
    int odsInt{toODS(i)};
    const std::uint8_t* ptr = reinterpret_cast<const std::uint8_t*>(&odsInt);
    for (std::vector<std::uint8_t>::size_type i = 0; i < sizeof(int); i++) {
        buffer.push_back(ptr[i]);
    }
}
}  // namespace

SecureArray MetaData::serialize() const {
    std::vector<std::uint8_t> buffer{};
    buffer.reserve(1024);

    for (auto item : _dataItems) {
        std::string key = item.first;
        addStringToBuffer(buffer, key);

        int value = item.second;
        addIntToBuffer(buffer, value);
    }

    SecureArray::size_type bufferSize{
        static_cast<SecureArray::size_type>(buffer.size())};

    SecureArray serialized{bufferSize};
    for (SecureArray::size_type i = 0; i < bufferSize; i++) {
        (*serialized)[i] = buffer[i];
    }

    return serialized;
}
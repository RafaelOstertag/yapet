#include <algorithm>
#include <cstring>

#include "passwordlistitem.hh"

using namespace yapet;

PasswordListItem::PasswordListItem() : _name{}, _encryptedRecord{} {}

PasswordListItem::PasswordListItem(const char* host,
                                   const SecureArray& encryptedRecord)
    : _name{PasswordRecord::NAME_SIZE}, _encryptedRecord{encryptedRecord} {
    auto stringLengthIncludingZero = std::strlen(host) + 1;
    auto len = stringLengthIncludingZero > PasswordRecord::NAME_SIZE
                   ? PasswordRecord::NAME_SIZE
                   : stringLengthIncludingZero;

    std::memcpy(*_name, host, len);
    (*_name)[PasswordRecord::NAME_SIZE - 1] = '\0';
}

PasswordListItem::PasswordListItem(const PasswordListItem& item)
    : _name{item._name}, _encryptedRecord{item._encryptedRecord} {}
PasswordListItem& PasswordListItem::operator=(const PasswordListItem& item) {
    if (&item == this) {
        return *this;
    }

    _name = item._name;
    _encryptedRecord = item._encryptedRecord;

    return *this;
}

PasswordListItem::PasswordListItem(PasswordListItem&& item)
    : _name{std::move(item._name)},
      _encryptedRecord{std::move(item._encryptedRecord)} {}
PasswordListItem& PasswordListItem::operator=(PasswordListItem&& item) {
    if (&item == this) {
        return *this;
    }

    _name = std::move(item._name);
    _encryptedRecord = std::move(item._encryptedRecord);

    return *this;
}

bool PasswordListItem::operator==(const PasswordListItem& other) const {
    return std::strncmp(reinterpret_cast<const char*>(*_name),
                        reinterpret_cast<const char*>(*other._name),
                        _name.size()) == 0;
}

PasswordListItem::operator std::string() const {
    return std::string{reinterpret_cast<const char*>(*_name)};
}

bool yapet::operator<(const PasswordListItem& a, const PasswordListItem& b) {
    const char* aName = reinterpret_cast<const char*>(a.name());
    const char* bName = reinterpret_cast<const char*>(b.name());

    auto result{
        std::strncmp(aName, bName, std::min(a.nameSize(), b.nameSize()))};

    return result < 0;
}

bool yapet::operator>(const PasswordListItem& a, const PasswordListItem& b) {
    const char* aName = reinterpret_cast<const char*>(a.name());
    const char* bName = reinterpret_cast<const char*>(b.name());

    auto result{
        std::strncmp(aName, bName, std::min(a.nameSize(), b.nameSize()))};

    return result > 0;
}
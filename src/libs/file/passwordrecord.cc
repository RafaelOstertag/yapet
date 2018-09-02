#include "passwordrecord.hh"

#include <algorithm>
#include <cstring>

using namespace yapet;

namespace {
inline void copyUint8(std::uint8_t* dest, const std::uint8_t* src, int srcLen,
                      int maxLen) {
    assert(dest != nullptr);
    assert(src != nullptr);

    auto len = std::min(srcLen, maxLen);
    std::memcpy(dest, src, len);
    dest[maxLen - 1] = '\0';
}

inline void copyString(std::uint8_t* dest, const char* src, size_t maxLen) {
    assert(dest != nullptr);
    assert(src != nullptr);

    copyUint8(dest, reinterpret_cast<const std::uint8_t*>(src),
              std::strlen(src) + 1, maxLen);
}

}  // namespace

PasswordRecord::PasswordRecord()
    : _name{NAME_SIZE},
      _host{HOST_SIZE},
      _username{USERNAME_SIZE},
      _password{PASSWORD_SIZE},
      _comment{COMMENT_SIZE} {}

PasswordRecord::PasswordRecord(const SecureArray& serialized) { assert(false); }

PasswordRecord::PasswordRecord(const PasswordRecord& p)
    : _name{p._name},
      _host{p._host},
      _username{p._username},
      _password{p._password},
      _comment{p._comment} {}

PasswordRecord::PasswordRecord(PasswordRecord&& p)
    : _name{std::move(p._name)},
      _host{std::move(p._host)},
      _username{std::move(p._username)},
      _password{std::move(p._password)},
      _comment{std::move(p._comment)} {}

PasswordRecord& PasswordRecord::operator=(const PasswordRecord& p) {
    if (&p == this) {
        return *this;
    }

    _name = p._name;
    _host = p._host;
    _username = p._username;
    _password = p._password;
    _comment = p._comment;

    return *this;
}

PasswordRecord& PasswordRecord::operator=(PasswordRecord&& p) {
    if (&p == this) {
        return *this;
    }

    _name = std::move(p._name);
    _host = std::move(p._host);
    _username = std::move(p._username);
    _password = std::move(p._password);
    _comment = std::move(p._comment);

    return *this;
}

SecureArray PasswordRecord::serialize() const {
    SecureArray serialized{TOTAL_SIZE};

    auto positionInBuffer = 0;

    std::memcpy(*serialized + positionInBuffer, *_name, NAME_SIZE);
    positionInBuffer += NAME_SIZE;

    std::memcpy(*serialized + positionInBuffer, *_host, HOST_SIZE);
    positionInBuffer += HOST_SIZE;

    std::memcpy(*serialized + positionInBuffer, *_username, USERNAME_SIZE);
    positionInBuffer += USERNAME_SIZE;

    std::memcpy(*serialized + positionInBuffer, *_password, PASSWORD_SIZE);
    positionInBuffer += PASSWORD_SIZE;

    std::memcpy(*serialized + positionInBuffer, *_comment, COMMENT_SIZE);
    positionInBuffer += COMMENT_SIZE;

    return serialized;
}

void PasswordRecord::name(const char* name) {
    copyString(*_name, name, NAME_SIZE);
}
void PasswordRecord::name(const std::uint8_t* name, int l) {
    copyUint8(*_name, name, l, NAME_SIZE);
}

void PasswordRecord::host(const char* host) {
    copyString(*_host, host, HOST_SIZE);
}
void PasswordRecord::host(const std::uint8_t* host, int l) {
    copyUint8(*_host, host, l, HOST_SIZE);
}

void PasswordRecord::username(const char* username) {
    copyString(*_username, username, USERNAME_SIZE);
}
void PasswordRecord::username(const std::uint8_t* username, int l) {
    copyUint8(*_username, username, l, USERNAME_SIZE);
}

void PasswordRecord::password(const char* password) {
    copyString(*_password, password, PASSWORD_SIZE);
}
void PasswordRecord::password(const std::uint8_t* password, int l) {
    copyUint8(*_password, password, l, PASSWORD_SIZE);
}

void PasswordRecord::comment(const char* comment) {
    copyString(*_comment, comment, COMMENT_SIZE);
}
void PasswordRecord::comment(const std::uint8_t* comment, int l) {
    copyUint8(*_comment, comment, l, COMMENT_SIZE);
}
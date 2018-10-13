#include <iostream>
#include <map>

#include "pwgen.hh"

int main() {
    std::map<std::uint8_t, int> distribution;

    yapet::pwgen::PasswordGenerator pwgen{yapet::pwgen::LETTERS |
                                          yapet::pwgen::DIGITS};
    auto password{pwgen.generatePassword(1000000)};

    for (yapet::SecureArray::size_type i = 0; i < password.size() - 1; i++) {
        distribution[password[i]]++;
    }

    for (std::pair<int, int> p : distribution) {
        std::cout << static_cast<char>(p.first) << "," << p.second << "\n";
    }
}
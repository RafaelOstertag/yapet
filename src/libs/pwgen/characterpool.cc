#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>

#include "characterpool.hh"
#include "intl.h"

using namespace yapet::pwgen;

const std::string yapet::pwgen::letters{
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const std::string yapet::pwgen::digits{"0123456789"};
const std::string yapet::pwgen::punctuation{".,;:-!?'"};
const std::string yapet::pwgen::special{"_+\"*%&/()[]={}<>"};
const std::string yapet::pwgen::other{"@#\\|$~`^"};

std::string yapet::pwgen::getPools(int pools) {
    std::string characterPools;
    if (isLetters(pools)) {
        characterPools += letters;
    }
    if (isDigits(pools)) {
        characterPools += digits;
    }
    if (isPunct(pools)) {
        characterPools += punctuation;
    }
    if (isSpecial(pools)) {
        characterPools += special;
    }
    if (isOther(pools)) {
        characterPools += other;
    }

    if (characterPools.size() == 0) {
        throw std::out_of_range{_("No character pools selected")};
    }

    return characterPools;
}
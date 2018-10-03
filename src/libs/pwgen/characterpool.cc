#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>

#include "characterpool.hh"
#include "intl.h"

using namespace yapet::pwgen;

const CharacterPool yapet::pwgen::letters{
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const CharacterPool yapet::pwgen::digits{"0123456789"};
const CharacterPool yapet::pwgen::punctuation{".,;:-!?'"};
const CharacterPool yapet::pwgen::special{"_+\"*%&/()[]={}<>"};
const CharacterPool yapet::pwgen::other{"@#\\|$~`^"};

std::vector<CharacterPool> yapet::pwgen::getPools(int pools) {
    std::vector<CharacterPool> list{};

    if (isLetters(pools)) {
        list.push_back(letters);
    }
    if (isDigits(pools)) {
        list.push_back(digits);
    }
    if (isPunct(pools)) {
        list.push_back(punctuation);
    }
    if (isSpecial(pools)) {
        list.push_back(special);
    }
    if (isOther(pools)) {
        list.push_back(other);
    }

    if (list.size() == 0) {
        throw std::out_of_range{_("No character pools selected")};
    }

    return list;
}
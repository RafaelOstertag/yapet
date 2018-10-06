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

CharacterPool yapet::pwgen::getPools(int pools) {
    CharacterPool characterPools;
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

    if (characterPools.characters().size() == 0) {
        throw std::out_of_range{_("No character pools selected")};
    }

    return characterPools;
}
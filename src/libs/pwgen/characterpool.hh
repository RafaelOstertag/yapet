#ifndef _CHARACTERPOOL_HH
#define _CHARACTERPOOL_HH

#include <string>
#include <vector>

namespace yapet {
namespace pwgen {

/**
 * @brief Available character pools
 *
 * Pools to choose from.
 */
enum POOLS {
    /**
     * Letters: a-z, A-Z
     */
    LETTERS = (1 << 0),
    /**
     * Digits: 0-9
     */
    DIGITS = (1 << 1),
    /**
     * Punctuation characters: comma, semicolon, etc.
     */
    PUNCT = (1 << 2),
    /**
     * Special characters: #,(,), etc.
     */
    SPECIAL = (1 << 3),
    /**
     * Other characters
     */
    OTHER = (1 << 4),
    ALL = LETTERS | DIGITS | PUNCT | SPECIAL | OTHER
};

constexpr bool isLetters(int v) { return (v & LETTERS); }
constexpr bool isDigits(int v) { return (v & DIGITS); }
constexpr bool isPunct(int v) { return (v & PUNCT); }
constexpr bool isSpecial(int v) { return (v & SPECIAL); }
constexpr bool isOther(int v) { return (v & OTHER); }
constexpr bool isAll(int v) { return (v & ALL); }

class CharacterPool {
   private:
    std::string _characters;

   public:
    CharacterPool() : _characters{} {}
    CharacterPool(const std::string& pool) : _characters{pool} {}
    CharacterPool(const CharacterPool& cp) : _characters{cp._characters} {}
    CharacterPool(CharacterPool&& cp)
        : _characters{std::move(cp._characters)} {}

    CharacterPool& operator=(const CharacterPool& cp) {
        if (this == &cp) return *this;

        _characters = cp._characters;

        return *this;
    }

    CharacterPool& operator=(CharacterPool&& cp) {
        if (this == &cp) return *this;

        _characters = std::move(cp._characters);

        return *this;
    }

    bool operator==(const CharacterPool& other) {
        if (this == &other) return true;

        return _characters == other._characters;
    }

    /**
     * Return the character at position num % number_of_chars_in_pool.
     */
    char operator[](std::string::size_type num) const {
        return _characters[num % _characters.size()];
    }
};

extern const CharacterPool letters;
extern const CharacterPool digits;
extern const CharacterPool punctuation;
extern const CharacterPool special;
extern const CharacterPool other;

std::vector<CharacterPool> getPools(int pools);

}  // namespace pwgen
}  // namespace yapet

#endif
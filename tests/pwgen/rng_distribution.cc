#include <iostream>
#include <map>

#include <random>
#include "rng.hh"

constexpr auto ROUNDS{1000000};

int main() {
    std::map<int, int> distribution;
    yapet::pwgen::Rng rng{1, 100};

    for (int i = 0; i < ROUNDS; i++) {
        distribution[rng.getNextInt()]++;
    }

    for (std::pair<int, int> p : distribution) {
        std::cout << std::to_string(p.first) << "," << p.second << "\n";
    }
}
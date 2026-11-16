// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP

#include <atomic>
#include <vector>

// Hands out consecutive ids. Get the one instance with IdGenerator::instance().
class IdGenerator {
public:
    static IdGenerator &instance();
    static int constructions();

    IdGenerator(const IdGenerator &) = delete; // no copies
    IdGenerator &operator=(const IdGenerator &) = delete;

    long next_id() { return next_++; }

private:
    IdGenerator(); // private: only instance() can make one
    std::atomic<long> next_{1};
};

std::vector<long> singleton_report(int calls);

#endif

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

#include "id_generator.hpp"

#include <algorithm>

namespace {
std::atomic<int> construction_count{0};
}

IdGenerator::IdGenerator() {
    construction_count++;
}

IdGenerator &IdGenerator::instance() {
    // A local static is made on the first call, and the language makes this
    // thread-safe: other threads wait until the initialization is done.
    static IdGenerator g;
    return g;
}

int IdGenerator::constructions() {
    return construction_count.load();
}

std::vector<long> singleton_report(int calls) {
    std::vector<const IdGenerator *> seen;
    long first = 0;
    long last = 0;
    for (int i = 0; i < calls; i++) {
        IdGenerator &g = IdGenerator::instance();
        if (std::find(seen.begin(), seen.end(), &g) == seen.end()) {
            seen.push_back(&g);
        }
        last = g.next_id();
        if (i == 0) {
            first = last;
        }
    }
    return {last - first, IdGenerator::constructions(), static_cast<long>(seen.size())};
}

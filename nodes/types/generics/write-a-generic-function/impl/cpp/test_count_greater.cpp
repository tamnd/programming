// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

#include <climits>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

#include "count_greater.hpp"

namespace {

struct test_case {
    const char* name;
    std::vector<int> items;
    int limit;
    std::size_t want;
};

const test_case cases[] = {
    {"some", {34, 50, 25, 100, 65}, 50, 2},
    {"all", {3, 4, 5}, 2, 3},
    {"none", {3, 4, 5}, 9, 0},
    {"empty", {}, 0, 0},
    {"equal-not-counted", {5, 5, 5}, 5, 0},
    {"duplicates", {7, 1, 7, 7}, 6, 3},
    {"negative", {-3, -1, 0, 2}, -2, 3},
    {"min-limit", {INT_MIN, 0, INT_MAX}, INT_MIN, 2},
    {"max-limit", {INT_MAX, 1}, INT_MAX, 0},
    {"single", {1}, 0, 1},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const std::size_t got = generic::count_greater_int(c.items, c.limit);
        if (got != c.want) {
            std::printf("FAIL %s: got %zu, want %zu\n", c.name, got, c.want);
            ++failures;
        }
    }
    const std::vector<std::string> words{"pear", "apple", "plum"};
    const std::size_t got = generic::count_greater(words, std::string("orange"));
    if (got != 2) {
        std::printf("FAIL strings: got %zu, want 2\n", got);
        ++failures;
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}

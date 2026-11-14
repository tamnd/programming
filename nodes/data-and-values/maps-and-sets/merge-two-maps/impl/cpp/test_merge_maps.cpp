// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

#include "merge_maps.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

static int failures = 0;

using Map = std::unordered_map<std::string, int>;
using Pairs = std::vector<std::pair<std::string, int>>;

static void check(std::string_view name, const Map &base, const Map &over, const Pairs &want) {
    if (merge_maps(base, over) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("prices", Map{{"apple", 3}, {"pear", 5}}, Map{{"pear", 6}, {"fig", 2}}, Pairs{{"apple", 3}, {"fig", 2}, {"pear", 6}});
    check("disjoint", Map{{"a", 1}}, Map{{"b", 2}}, Pairs{{"a", 1}, {"b", 2}});
    check("empty-base", Map{}, Map{{"a", 1}}, Pairs{{"a", 1}});
    check("empty-override", Map{{"a", 1}}, Map{}, Pairs{{"a", 1}});
    check("both-empty", Map{}, Map{}, Pairs{});
    check("same-keys", Map{{"a", 1}, {"b", 2}}, Map{{"a", 9}, {"b", 8}}, Pairs{{"a", 9}, {"b", 8}});
    check("zero-wins", Map{{"a", 5}}, Map{{"a", 0}}, Pairs{{"a", 0}});
    check("case", Map{{"a", 1}}, Map{{"A", 2}}, Pairs{{"A", 2}, {"a", 1}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

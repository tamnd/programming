// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

#include "lookup.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

static int failures = 0;

using Map = std::unordered_map<std::string, int>;
using Keys = std::vector<std::string>;
using Ints = std::vector<int>;

static void check(std::string_view name, const Map &table, const Keys &keys, int fallback, const Ints &want) {
    const Map before = table;
    if (lookup(table, keys, fallback) != want || table != before) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("prices", Map{{"apple", 3}, {"pear", 5}}, Keys{"apple", "fig", "pear"}, 0, Ints{3, 0, 5});
    check("negative-fallback", Map{{"a", 1}}, Keys{"b", "a"}, -1, Ints{-1, 1});
    check("empty-table", Map{}, Keys{"a", "b"}, 7, Ints{7, 7});
    check("empty-keys", Map{{"a", 1}}, Keys{}, 0, Ints{});
    check("zero-value", Map{{"a", 0}}, Keys{"a", "b"}, 5, Ints{0, 5});
    check("repeat", Map{{"a", 1}}, Keys{"b", "b", "a"}, 2, Ints{2, 2, 1});
    check("case", Map{{"a", 1}}, Keys{"A"}, 0, Ints{0});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

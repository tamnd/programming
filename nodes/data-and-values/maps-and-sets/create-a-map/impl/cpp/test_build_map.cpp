// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

#include "build_map.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

static int failures = 0;

using Pairs = std::vector<std::pair<std::string, int>>;

static void check(std::string_view name, const Pairs &pairs, const std::vector<std::string> &remove, const Pairs &want) {
    if (build_map(pairs, remove) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("fruits", Pairs{{"apple", 3}, {"pear", 5}, {"fig", 2}}, std::vector<std::string>{}, Pairs{{"apple", 3}, {"fig", 2}, {"pear", 5}});
    check("update-and-delete", Pairs{{"a", 1}, {"b", 2}, {"a", 7}, {"c", 3}}, std::vector<std::string>{"b"}, Pairs{{"a", 7}, {"c", 3}});
    check("empty", Pairs{}, std::vector<std::string>{}, Pairs{});
    check("replace", Pairs{{"k", 1}, {"k", 2}, {"k", 3}}, std::vector<std::string>{}, Pairs{{"k", 3}});
    check("delete-absent", Pairs{{"a", 1}}, std::vector<std::string>{"z"}, Pairs{{"a", 1}});
    check("delete-all", Pairs{{"a", 1}, {"b", 2}}, std::vector<std::string>{"a", "b"}, Pairs{});
    check("delete-twice", Pairs{{"a", 1}, {"b", 2}}, std::vector<std::string>{"a", "a"}, Pairs{{"b", 2}});
    check("zero-value", Pairs{{"a", 0}, {"b", -4}}, std::vector<std::string>{}, Pairs{{"a", 0}, {"b", -4}});
    check("case", Pairs{{"b", 1}, {"B", 2}, {"a", 3}, {"A", 4}}, std::vector<std::string>{}, Pairs{{"A", 4}, {"B", 2}, {"a", 3}, {"b", 1}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

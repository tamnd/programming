// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

#include "score_lines.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::unordered_map<std::string, int> &scores, const std::vector<std::string> &want) {
    if (score_lines(scores) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("scores", {{"bob", 7}, {"amy", 9}, {"cal", 4}}, {"amy=9", "bob=7", "cal=4", "total=20"});
    check("one", {{"x", 1}}, {"x=1", "total=1"});
    check("empty", {}, {"total=0"});
    check("case", {{"b", 1}, {"B", 2}, {"a", 3}, {"A", 4}}, {"A=4", "B=2", "a=3", "b=1", "total=10"});
    check("negative", {{"a", -5}, {"b", 5}}, {"a=-5", "b=5", "total=0"});
    check("prefix", {{"abc", 3}, {"a", 2}, {"ab", 1}}, {"a=2", "ab=1", "abc=3", "total=6"});
    check("reverse", {{"j", 1}, {"i", 2}, {"h", 3}, {"g", 4}, {"f", 5}, {"e", 6}, {"d", 7}, {"c", 8}, {"b", 9}, {"a", 10}}, {"a=10", "b=9", "c=8", "d=7", "e=6", "f=5", "g=4", "h=3", "i=2", "j=1", "total=55"});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

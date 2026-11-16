// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

#include "birthday.hpp"

#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

using People = std::vector<Person>;

static void check(std::string_view label, const People &people, const std::string &name, const People &want) {
    const People before = people;
    if (birthday(people, name) != want || people != before) {
        std::cerr << "case " << label << " failed\n";
        failures++;
    }
}

int main() {
    check("first", People{{"Ada", 36}, {"Alan", 41}}, "Ada", People{{"Ada", 37}, {"Alan", 41}});
    check("second", People{{"Ada", 36}, {"Alan", 41}}, "Alan", People{{"Ada", 36}, {"Alan", 42}});
    check("empty", People{}, "Ada", People{});
    check("not-found", People{{"Ada", 36}}, "Grace", People{{"Ada", 36}});
    check("duplicate-name", People{{"Ada", 36}, {"Alan", 41}, {"Ada", 9}}, "Ada", People{{"Ada", 37}, {"Alan", 41}, {"Ada", 10}});
    check("zero-age", People{{"Baby", 0}}, "Baby", People{{"Baby", 1}});
    check("non-ascii", People{{"Zo\xc3\xab", 20}, {"Zoe", 30}}, "Zo\xc3\xab", People{{"Zo\xc3\xab", 21}, {"Zoe", 30}});
    check("case", People{{"ada", 36}}, "Ada", People{{"ada", 36}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

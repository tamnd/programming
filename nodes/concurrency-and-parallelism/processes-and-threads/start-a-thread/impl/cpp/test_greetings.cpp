// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

#include "greetings.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::string_view>& names,
                  const std::vector<std::string>& want) {
    if (greetings::thread_greetings(names) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("two", {"Ada", "Linus"}, {"hello from Ada", "hello from Linus"});
    check("three", {"main", "worker", "logger"}, {"hello from main", "hello from worker", "hello from logger"});
    check("empty", {}, {});
    check("single", {"solo"}, {"hello from solo"});
    check("empty-name", {""}, {"hello from "});
    check("duplicate", {"twin", "twin"}, {"hello from twin", "hello from twin"});
    check("unicode", {"Zoë", "李雷"}, {"hello from Zoë", "hello from 李雷"});

    std::vector<std::string> many;
    std::vector<std::string> want;
    for (int i = 0; i < 32; i++) {
        many.push_back("t" + std::to_string(i));
        want.push_back("hello from t" + std::to_string(i));
    }
    check("many", std::vector<std::string_view>(many.begin(), many.end()), want);

    if (failures == 0) {
        std::cout << "all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}

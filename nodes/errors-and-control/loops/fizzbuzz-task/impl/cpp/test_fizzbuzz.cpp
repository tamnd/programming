// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

#include "fizzbuzz.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, int n, const std::vector<std::string> &want) {
    if (fizzbuzz(n) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("fifteen", 15, {"1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz"});
    check("thirty", 30, {"1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz", "16", "17", "Fizz", "19", "Buzz", "Fizz", "22", "23", "Fizz", "Buzz", "26", "Fizz", "28", "29", "FizzBuzz"});
    check("zero", 0, {});
    check("one", 1, {"1"});
    check("three", 3, {"1", "2", "Fizz"});
    check("five", 5, {"1", "2", "Fizz", "4", "Buzz"});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

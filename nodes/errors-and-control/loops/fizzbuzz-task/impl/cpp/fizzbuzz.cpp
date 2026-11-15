// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

#include "fizzbuzz.hpp"

#include <string>
#include <vector>

std::vector<std::string> fizzbuzz(int n) {
    std::vector<std::string> out;
    for (int i = 1; i <= n; i++) {
        // Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
        if (i % 15 == 0) {
            out.push_back("FizzBuzz");
        } else if (i % 3 == 0) {
            out.push_back("Fizz");
        } else if (i % 5 == 0) {
            out.push_back("Buzz");
        } else {
            out.push_back(std::to_string(i));
        }
    }
    return out;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

#include "words.hpp"

#include <iostream>
#include <string>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, std::string_view text, int want) {
    if (words::word_count(text) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("three-words", "the quick fox", 3);
    check("one-word", "hello", 1);
    check("punctuation", "Hello, world! How are you?", 5);
    check("empty", "", 0);
    check("blank", " \t\n ", 0);
    check("mixed-whitespace", "  two\twords\r\nand three  ", 4);
    check("non-ascii", "na\xc3\xafve caf\xc3\xa9 \xe6\x9d\xb1\xe4\xba\xac", 3);

    const std::string s = "one two";  // a std::string converts to std::string_view
    if (words::word_count(s) != 2) {
        std::cerr << "case std-string failed\n";
        failures++;
    }
    if (failures == 0) {
        std::cout << "all cases passed\n";
    }
    return failures == 0 ? 0 : 1;
}

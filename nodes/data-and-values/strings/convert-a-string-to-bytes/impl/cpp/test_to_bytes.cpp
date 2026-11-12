// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

#include "to_bytes.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, std::string_view s, const std::vector<std::uint8_t> &want) {
    if (to_bytes(s) != want) {
        std::cerr << "case " << name << " fails\n";
        failures++;
    }
}

// The encoding is UTF-8. U+00E9 is 2 bytes, U+20AC is 3 bytes and U+1F642 is 4 bytes. In Latin-1, U+00FF would be 1 byte.
int main() {
    check("ascii", "abc", {97, 98, 99});
    check("letter", "A", {65});
    check("punct", "hi!", {104, 105, 33});
    check("empty", "", {});
    check("control", "\t\n", {9, 10});
    check("two-byte", "\xc3\xa9", {195, 169});
    check("latin-1-max", "\xc3\xbf", {195, 191});
    check("three-byte", "\xe2\x82\xac", {226, 130, 172});
    check("bom", "\xef\xbb\xbf", {239, 187, 191});
    check("astral", "\xf0\x9f\x99\x82", {240, 159, 153, 130});
    check("combining", "e\xcc\x81", {101, 204, 129});
    check("mixed", "a\xc3\xa9\xf0\x9f\x99\x82", {97, 195, 169, 240, 159, 153, 130});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

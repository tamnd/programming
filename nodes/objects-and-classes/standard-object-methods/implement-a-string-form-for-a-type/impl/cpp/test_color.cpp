// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

#include "color.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using color::Color;

static int failures = 0;

static void check(std::string_view name, const std::vector<int>& reds, const std::vector<int>& greens,
                  const std::vector<int>& blues, const std::vector<std::string>& want) {
    if (color::format_colors(reds, greens, blues) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

static void expect(std::string_view name, bool ok) {
    if (!ok) {
        std::cerr << "check " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("orange", {255}, {128}, {0}, {"#FF8000"});
    check("three", {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {"#FF0000", "#00FF00", "#0000FF"});
    check("mixed", {18, 171}, {52, 205}, {86, 239}, {"#123456", "#ABCDEF"});
    check("empty", {}, {}, {}, {});
    check("single", {1}, {2}, {3}, {"#010203"});
    check("zero", {0}, {0}, {0}, {"#000000"});
    check("max", {255}, {255}, {255}, {"#FFFFFF"});
    check("padding", {10, 0}, {11, 15}, {12, 16}, {"#0A0B0C", "#000F10"});

    Color c(255, 128, 0);
    expect("to_string", c.to_string() == "#FF8000");
    std::ostringstream s;
    s << "color " << c << ' ' << 42;
    expect("stream", s.str() == "color #FF8000 42");
    bool threw = false;
    try {
        [[maybe_unused]] Color bad(256, 0, 0);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    expect("bad value", threw);

    if (failures == 0) {
        std::cout << "all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}

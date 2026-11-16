// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

#include "version.hpp"

#include <compare>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using version::Version;

static int failures = 0;

static void check(std::string_view name, const std::vector<std::string>& versions,
                  const std::vector<std::string>& want) {
    if (version::sort_versions(versions) != want) {
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
    check("simple", {"1.2.0", "1.0.0", "1.1.0"}, {"1.0.0", "1.1.0", "1.2.0"});
    check("fields-in-order", {"2.0.0", "1.9.9", "1.10.0"}, {"1.9.9", "1.10.0", "2.0.0"});
    check("patch", {"0.1.10", "0.1.2", "0.1.1"}, {"0.1.1", "0.1.2", "0.1.10"});
    check("empty", {}, {});
    check("single", {"3.4.5"}, {"3.4.5"});
    check("numeric-not-text", {"10.0.0", "9.0.0", "1.0.0"}, {"1.0.0", "9.0.0", "10.0.0"});
    check("duplicates", {"1.0.0", "0.9.0", "1.0.0"}, {"0.9.0", "1.0.0", "1.0.0"});
    check("zero", {"0.0.1", "0.0.0"}, {"0.0.0", "0.0.1"});
    check("limit", {"1000000.0.0", "999999.1000000.1000000"}, {"999999.1000000.1000000", "1000000.0.0"});

    Version a{1, 2, 3}, b{1, 10, 0};
    expect("less", a < b && b > a && a <= a && a >= a);
    expect("three-way", (a <=> b) == std::strong_ordering::less);
    expect("equal", a == Version{1, 2, 3} && a != b);
    expect("parse", Version::parse("1.10.0") == Version{1, 10, 0});
    expect("parse two parts", !Version::parse("1.2"));
    expect("parse sign", !Version::parse("1.-2.0"));
    std::set<Version> set{{2, 0, 0}, {1, 0, 0}, {1, 0, 0}};
    expect("set", set.size() == 2 && *set.begin() == Version{1, 0, 0});
    bool threw = false;
    try {
        version::sort_versions({"1.0.0", "x"});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    expect("bad input", threw);

    if (failures == 0) {
        std::cout << "all cases passed\n";
    }
    return failures == 0 ? 0 : 1;
}

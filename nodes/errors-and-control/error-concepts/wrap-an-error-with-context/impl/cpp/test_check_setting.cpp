// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

#include "check_setting.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, const std::string &where, const std::string &text, const std::string &want) {
    std::string got = check_setting(where, text);
    if (got != want) {
        std::cerr << "case " << name << " failed: got " << got << "\n";
        failures++;
    }
}

int main() {
    check("valid", "app.conf:3", "8080", "port 8080");
    check("not-a-number", "app.conf:3", "x", "error: app.conf:3: read port: not a number: 'x'");
    check("too-large", "app.conf:7", "70000", "error: app.conf:7: read port: out of range: 70000");
    check("empty", "app.conf:1", "", "error: app.conf:1: read port: not a number: ''");
    check("zero", "app.conf:2", "0", "error: app.conf:2: read port: out of range: 0");
    check("largest", "app.conf:2", "65535", "port 65535");
    check("leading-space", "app.conf:4", " 80", "error: app.conf:4: read port: not a number: ' 80'");
    check("overflow", "app.conf:5", "99999999999999999999", "error: app.conf:5: read port: out of range: 99999999999999999999");
    check("negative", "app.conf:6", "-1", "error: app.conf:6: read port: not a number: '-1'");

    bool nested = false;
    try {
        read_port("app.conf:3", "x");
    } catch (const std::runtime_error &e) {
        try {
            std::rethrow_if_nested(e);
        } catch (const std::invalid_argument &) {
            nested = true;
        }
    }
    if (!nested) {
        std::cerr << "case cause failed\n";
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

#include "check_port.hpp"

#include <iostream>
#include <string>
#include <string_view>

static int failures = 0;

static void check(std::string_view name, const std::string &text, const std::string &want) {
    std::string got = check_port(text);
    if (got != want) {
        std::cerr << "case " << name << " failed: got " << got << "\n";
        failures++;
    }
}

int main() {
    check("valid", "8080", "port 8080");
    check("not-a-number", "x80", "invalid port 'x80': not a number");
    check("too-large", "65536", "invalid port '65536': out of range");
    check("empty", "", "invalid port '': empty");
    check("zero", "0", "invalid port '0': out of range");
    check("one", "1", "port 1");
    check("largest", "65535", "port 65535");
    check("leading-zeros", "0080", "port 80");
    check("negative", "-1", "invalid port '-1': not a number");
    check("leading-space", " 80", "invalid port ' 80': not a number");
    check("overflow", "99999999999999999999", "invalid port '99999999999999999999': out of range");

    try {
        parse_port("x80");
        std::cerr << "case fields failed: no exception\n";
        failures++;
    } catch (const PortError &e) {
        if (e.text() != "x80" || e.reason() != "not a number") {
            std::cerr << "case fields failed\n";
            failures++;
        }
    }
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

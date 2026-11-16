// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

#include "read_ports.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::string> &texts, const std::vector<int> &want_ports,
                  const std::string &want_log) {
    std::ostringstream log;
    std::vector<int> ports = read_ports(texts, log);
    if (ports != want_ports || log.str() != want_log) {
        std::cerr << "case " << name << " failed: log " << log.str() << "\n";
        failures++;
    }
}

int main() {
    check("one-bad", {"8080", "x", "443"}, {8080, 443}, "level=ERROR msg=\"read port failed\" line=2 err=\"not a number: 'x'\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n");
    check("all-good", {"80", "443"}, {80, 443}, "level=INFO msg=\"ports read\" ok=2 total=2\n");
    check("out-of-range", {"70000"}, {}, "level=ERROR msg=\"read port failed\" line=1 err=\"out of range: 70000\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("empty-list", {}, {}, "level=INFO msg=\"ports read\" ok=0 total=0\n");
    check("empty-text", {""}, {}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: ''\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("boundary", {"1", "65535", "0"}, {1, 65535}, "level=ERROR msg=\"read port failed\" line=3 err=\"out of range: 0\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n");
    check("newline", {"80\nlevel=INFO msg=\"all good\""}, {}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\nlevel=INFO msg=\\\"all good\\\"'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("backslash", {"C:\\port"}, {}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: 'C:\\\\port'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("carriage-return", {"80\r"}, {}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\r'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    check("tab", {"8\t0"}, {}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '8\\t0'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n");
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

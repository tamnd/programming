// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "read_lines.hpp"

namespace {

const std::filesystem::path input = "read_lines_input.txt";

int failures = 0;

void check(std::string_view name, bool ok) {
    if (!ok) {
        std::fprintf(stderr, "case %.*s failed\n", static_cast<int>(name.size()), name.data());
        ++failures;
    }
}

}  // namespace

int main() {
    struct Case {
        std::string name, data;
        std::vector<std::string> want;
    };
    const std::string long_line(1 << 20, 'x');
    const std::vector<Case> cases = {
        {"lf", "alpha\nbeta\ngamma\n", {"alpha", "beta", "gamma"}},
        {"blank-lines", "first\n\n\nlast line\n", {"first", "", "", "last line"}},
        {"empty", "", {}},
        {"only-newline", "\n", {""}},
        {"no-final-newline", "one\ntwo\nthree", {"one", "two", "three"}},
        {"crlf", "one\r\ntwo\r\nthree\r\n", {"one", "two", "three"}},
        {"crlf-no-final-newline", "one\r\ntwo\r\nthree", {"one", "two", "three"}},
        {"mixed-endings", "one\r\ntwo\nthree\r\n", {"one", "two", "three"}},
        {"trailing-blank-line", "last\n\n", {"last", ""}},
        {"whitespace", "  two spaces\ttab \n", {"  two spaces\ttab "}},
        {"backslashes", "C:\\temp\\new\n", {"C:\\temp\\new"}},
        {"non-ascii",
         "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n",
         {"naïve café", "日本語のテキスト", "🙂🙂🙂🙂🙂🙂"}},
        {"long-line", long_line + "\nend\n", {long_line, "end"}},
    };
    for (const auto& c : cases) {
        std::istringstream in(c.data);
        check(c.name, read_lines(in) == c.want);

        // The same case on the real standard input. std::cin reads through
        // the C stream stdin, so freopen gives it the new file.
        {
            std::ofstream out(input, std::ios::binary);
            out << c.data;
        }
        if (std::freopen(input.c_str(), "rb", stdin) == nullptr) return EXIT_FAILURE;
        std::cin.clear();  // clears the eofbit of the case before
        check(c.name, read_lines() == c.want);
    }

    std::filesystem::remove(input);
    if (failures > 0) return EXIT_FAILURE;
    std::puts("all cases pass");
}

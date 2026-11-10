// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-a-subprocess/.

#include "run.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

struct TestCase {
    std::string name;
    std::vector<std::string> argv;
    RunResult want;
};

int main() {
    const std::vector<TestCase> cases{
        {"hello", {"printf", "hello"}, {0, "hello", ""}},
        {"two-arguments", {"echo", "one", "two"}, {0, "one two\n", ""}},
        {"exit-status", {"sh", "-c", "exit 3"}, {3, "", ""}},
        {"both-streams", {"sh", "-c", "echo out; echo err >&2; exit 2"}, {2, "out\n", "err\n"}},
        {"metacharacters",
         {"printf", "%s|", "a b", "$HOME", "*", "x;y", "\"q\"", "'s'"},
         {0, "a b|$HOME|*|x;y|\"q\"|'s'|", ""}},
        {"empty-argument", {"printf", "[%s]", ""}, {0, "[]", ""}},
        {"newline-argument", {"printf", "%s", "line one\nline two"}, {0, "line one\nline two", ""}},
        {"trailing-newlines", {"printf", "a\\n\\n"}, {0, "a\n\n", ""}},
        {"non-ascii", {"printf", "%s\\n", "xin chào, 日本語, 🙂"}, {0, "xin chào, 日本語, 🙂\n", ""}},
        {"stderr-only", {"sh", "-c", "echo warning >&2"}, {0, "", "warning\n"}},
        {"no-output", {"true"}, {0, "", ""}},
        {"false", {"false"}, {1, "", ""}},
        {"status-255", {"sh", "-c", "exit 255"}, {255, "", ""}},
        {"empty-stdin", {"sh", "-c", "if read -r line; then echo got; else echo eof; fi"}, {0, "eof\n", ""}},
    };
    int failures = 0;
    for (const auto &c : cases) {
        try {
            if (run(c.argv) != c.want) {
                std::cout << "FAIL " << c.name << '\n';
                failures++;
            }
        } catch (const std::exception &e) {
            std::cout << "FAIL " << c.name << ": " << e.what() << '\n';
            failures++;
        }
    }

    const std::string script = "head -c 1048576 /dev/zero | tr '\\0' x; head -c 1048576 /dev/zero | tr '\\0' y >&2";
    if (run({"sh", "-c", script}) != RunResult{0, std::string(1048576, 'x'), std::string(1048576, 'y')}) {
        std::cout << "FAIL large-output\n";
        failures++;
    }

    try {
        auto r = run({"no-such-program-7f3a"});
        // POSIX allows posix_spawnp to succeed and the child to exit with 127.
        if (r.status != 127) {
            std::cout << "FAIL missing-program: status " << r.status << '\n';
            failures++;
        }
    } catch (const std::system_error &e) {
        if (e.code() != std::errc::no_such_file_or_directory) {
            std::cout << "FAIL missing-program: " << e.what() << '\n';
            failures++;
        }
    }

    try {
        run({"sh", "-c", "kill -9 $$"});
        std::cout << "FAIL signal: no exception\n";
        failures++;
    } catch (const std::runtime_error &) {
    }

    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

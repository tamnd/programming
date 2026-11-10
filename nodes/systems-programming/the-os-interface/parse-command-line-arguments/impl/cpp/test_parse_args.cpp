// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

#include <cstdio>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

#include "parse_args.hpp"

namespace {

int failures = 0;

using Args = std::vector<std::string_view>;

void check(std::string_view name, const Args& args, const CountArgs& want) {
    auto got = parse_args(args);
    if (!got || *got != want) {
        std::fprintf(stderr, "case %.*s: wrong result\n", static_cast<int>(name.size()), name.data());
        ++failures;
    }
}

void check_error(std::string_view name, const Args& args) {
    if (parse_args(args)) {
        std::fprintf(stderr, "case %.*s: no error\n", static_cast<int>(name.size()), name.data());
        ++failures;
    }
}

}  // namespace

int main() {
    check("files-only", {"a.txt", "b.txt"}, {false, 10, {"a.txt", "b.txt"}});
    check("short-options", {"-v", "-n", "3", "a.txt"}, {true, 3, {"a.txt"}});
    check("long-options", {"--verbose", "--limit", "5", "a.txt", "b.txt"}, {true, 5, {"a.txt", "b.txt"}});
    check("no-args", {}, {false, 10, {}});
    check("long-equals", {"--limit=7", "notes.md"}, {false, 7, {"notes.md"}});
    check("repeated", {"-n", "3", "--limit", "5", "a.txt"}, {false, 5, {"a.txt"}});
    check("zero", {"-n", "0", "a.txt"}, {false, 0, {"a.txt"}});
    check("end-of-options", {"-v", "--", "-n", "--limit=4"}, {true, 10, {"-n", "--limit=4"}});
    check("only-terminator", {"--"}, {false, 10, {}});
    check("second-terminator", {"--", "--"}, {false, 10, {"--"}});
    check("dash", {"-"}, {false, 10, {"-"}});
    check("dash-after-option", {"-n", "2", "-", "b.txt"}, {false, 2, {"-", "b.txt"}});
    check("empty-arg", {""}, {false, 10, {""}});
    check("space", {"-v", "my file.txt"}, {true, 10, {"my file.txt"}});
    check("non-ascii", {"--limit=4", "日本語.txt", "café.md"}, {false, 4, {"日本語.txt", "café.md"}});

    check_error("unknown-short", {"-x", "a.txt"});
    check_error("unknown-long", {"--color", "a.txt"});
    check_error("missing-value", {"-n"});
    check_error("missing-value-long", {"--limit"});
    check_error("not-a-number", {"-n", "ten", "a.txt"});
    check_error("not-a-number-long", {"--limit=ten"});
    check_error("empty-value", {"--limit=", "a.txt"});
    check_error("verbose-with-value", {"--verbose=yes"});

    if (failures != 0) return EXIT_FAILURE;
    std::puts("ok");
    return EXIT_SUCCESS;
}

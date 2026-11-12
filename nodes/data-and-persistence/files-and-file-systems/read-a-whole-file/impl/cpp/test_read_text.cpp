// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "read_text.hpp"

namespace {

const std::filesystem::path input = "read_text_input.txt";

void write_file(std::string_view data) {
    std::ofstream out(input, std::ios::binary);
    out << data;
}

int failures = 0;

void check(std::string_view name, bool ok) {
    if (!ok) {
        std::fprintf(stderr, "case %.*s failed\n", static_cast<int>(name.size()), name.data());
        ++failures;
    }
}

}  // namespace

int main() {
    using namespace std::string_literals;
    std::string large;
    for (int i = 0; i < 65536; ++i) large += "0123456789abcdef";
    const std::vector<std::pair<std::string, std::string>> cases = {
        {"lf", "alpha\nbeta\n"},
        {"paragraphs", "first line\n\nsecond paragraph\n"},
        {"non-ascii", "naïve café\n日本語\n🙂\n"},
        {"empty", ""},
        {"no-final-newline", "one\ntwo"},
        {"trailing-newlines", "end\n\n\n"},
        {"crlf", "one\r\ntwo\r\n"},
        {"mixed-endings", "lf\ncrlf\r\ncr\rend"},
        {"bom", "\xEF\xBB\xBFhead\n"},
        {"whitespace", "  two spaces\ttab \n"},
        {"large", large},
    };
    for (const auto& [name, data] : cases) {
        write_file(data);
        auto got = read_text(input);
        check(name, got.has_value() && *got == data);
    }

    std::filesystem::remove(input);
    check("missing", read_text("no_such_file.txt").error_or(ReadError::read_failed) ==
                         ReadError::not_found);
    check("directory", read_text(".").error_or(ReadError::read_failed) == ReadError::not_a_file);

    const std::vector<std::pair<std::string, std::string>> invalid = {
        {"invalid-byte", "ok\n\xFF\xFE\n"},
        {"truncated-at-end", "ok\ncaf\xC3"},
        {"encoded-surrogate", "a\xED\xA0\x80"s + "b\n"},
        {"overlong", "\xC0\xAF\n"},
    };
    for (const auto& [name, data] : invalid) {
        write_file(data);
        check(name, read_text(input).error_or(ReadError::read_failed) == ReadError::invalid_utf8);
    }
    std::filesystem::remove(input);

    if (failures != 0) return EXIT_FAILURE;
    std::puts("all tests passed");
    return EXIT_SUCCESS;
}

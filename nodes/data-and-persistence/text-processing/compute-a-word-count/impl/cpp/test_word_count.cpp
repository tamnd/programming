// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "word_count.hpp"

namespace {

const std::filesystem::path input = "word_count_input.txt";

void write(const std::string& data) {
    std::ofstream out(input, std::ios::binary);
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
}

struct test_case {
    const char* name;
    std::string data;
    WordCount want;
};

struct invalid_case {
    const char* name;
    std::string data;
};

}  // namespace

int main() {
    std::string many;
    for (int i = 0; i < 100000; ++i) {
        many += "a b\n";  // 100000 lines: more than one buffer of 64 KiB
    }
    // A long line, and U+3000 at the end of the first 64 KiB.
    const std::string boundary = std::string(64 * 1024 - 1, 'x') + "\xe3\x80\x80y\n";

    // A hex escape takes all the hex digits after it, so a literal ends after
    // each escape that a letter from a to f follows.
    const std::vector<test_case> cases = {
        {"simple", "one two three\n", {1, 3, 14}},
        {"multiple-spaces", "  a   b  \n", {1, 2, 10}},
        {"tabs-and-lines", "a\tb\nc\n", {2, 3, 6}},
        {"empty", "", {0, 0, 0}},
        {"only-white-space", " \n\t\n", {2, 0, 4}},
        {"no-final-newline", "one two", {0, 2, 7}},
        {"crlf", "a b\r\nc\r\n", {2, 3, 8}},
        {"non-ascii", "na\xc3\xaf" "ve caf\xc3\xa9\n", {1, 2, 13}},
        {"unicode-spaces", "a\xc2\xa0" "b\xe3\x80\x80" "c\xe2\x80\xa8" "d\n", {1, 4, 13}},
        {"not-white-space", "a\xe2\x80\x8b" "b\xc2\xad" "c\xef\xbb\xbf" "d\n", {1, 1, 13}},
        {"control-separators", "a\x1c" "b\x1f" "c\n", {1, 1, 6}},
        {"nel", "a\xc2\x85" "b\n", {1, 2, 5}},
        {"many-lines", many, {100000, 200000, 400000}},
        {"chunk-boundary", boundary, {1, 2, 64 * 1024 + 4}},
    };
    for (const auto& c : cases) {
        write(c.data);
        const auto got = word_count(input);
        if (!got || *got != c.want) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            return 1;
        }
    }

    const std::vector<invalid_case> invalid = {
        {"invalid-bytes", "ok\xff\xfe\n"},
        {"truncated", "ok \xe2\x82"},
        {"surrogate", "a\xed\xa0\x80" "b\n"},
    };
    for (const auto& c : invalid) {
        write(c.data);
        const auto got = word_count(input);
        if (got || got.error() != CountError::invalid_utf8) {
            std::fprintf(stderr, "case %s: no invalid_utf8 error\n", c.name);
            return 1;
        }
    }
    std::filesystem::remove(input);

    const auto missing = word_count("word_count_missing.txt");
    if (missing || missing.error() != CountError::cannot_open) {
        std::fprintf(stderr, "case missing: no cannot_open error\n");
        return 1;
    }
    std::puts("ok");
    return 0;
}

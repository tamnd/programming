// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "word_frequencies.hpp"

namespace {

const std::filesystem::path input = "word_frequencies_input.txt";

void write(const std::string& data) {
    std::ofstream out(input, std::ios::binary);
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
}

std::string repeat(const std::string& unit, int times) {
    std::string s;
    for (int i = 0; i < times; i++) {
        s += unit;
    }
    return s;
}

struct test_case {
    const char* name;
    std::string data;
    std::size_t n;
    WordCounts want;
};

}  // namespace

int main() {
    const std::vector<test_case> cases = {
        {"simple", "the cat and the hat\n", 2, {{"the", 2}, {"and", 1}}},
        {"case", "The THE the tHe\n", 5, {{"the", 4}}},
        {"punctuation", "it's well-dressed, isn't it?\n", 10,
         {{"it", 2}, {"dressed", 1}, {"isn", 1}, {"s", 1}, {"t", 1}, {"well", 1}}},
        {"empty", "", 3, {}},
        {"no-letters", "123 ... 42\n-- !!\n", 3, {}},
        {"ties", "b a c b a c\n", 2, {{"a", 2}, {"b", 2}}},
        {"digits", "abc123def 42 x9\n", 10, {{"abc", 1}, {"def", 1}, {"x", 1}}},
        {"n-zero", "the cat and the hat\n", 0, {}},
        {"n-above-count", "x y x\n", 10, {{"x", 2}, {"y", 1}}},
        {"crlf", "one two\r\ntwo\r\n", 5, {{"two", 2}, {"one", 1}}},
        {"non-ascii", "caf\xc3\xa9 na\xc3\xafve cafe\n", 10, {{"caf", 1}, {"cafe", 1}, {"na", 1}, {"ve", 1}}},
        {"leading-separators", "  ...hello, world. Hello\n", 5, {{"hello", 2}, {"world", 1}}},
        {"no-final-newline", "dog cat dog", 5, {{"dog", 2}, {"cat", 1}}},
        {"not-utf8", "ok \xff\xfe ok\n", 5, {{"ok", 2}}},
        {"many-lines", repeat("a b a\n", 100000), 3, {{"a", 200000}, {"b", 100000}}},
        // One line of 100000 words: longer than the stream buffer.
        {"long-line", repeat("word ", 100000), 3, {{"word", 100000}}},
    };
    int failed = 0;
    for (const auto& c : cases) {
        write(c.data);
        auto got = word_frequencies(input, c.n);
        if (!got || *got != c.want) {
            std::fprintf(stderr, "case %s failed\n", c.name);
            failed++;
        }
    }
    auto missing = word_frequencies("no/such/dir/missing.txt", 3);
    if (missing || missing.error() != FrequencyError::cannot_open) {
        std::fprintf(stderr, "case not-found failed\n");
        failed++;
    }
    std::filesystem::remove(input);
    if (failed == 0) {
        std::puts("all cases pass");
    }
    return failed == 0 ? 0 : 1;
}

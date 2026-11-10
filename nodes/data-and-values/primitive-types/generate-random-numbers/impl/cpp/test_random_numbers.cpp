// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

#include <cstdint>
#include <cstdio>
#include <random>
#include <regex>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

#include "random_numbers.hpp"

namespace {

struct range_case {
    const char* name;
    std::vector<std::uint32_t> words;
    std::size_t nwords;
    std::int64_t lo, hi;
    std::size_t count;
    std::vector<std::int64_t> want;
};

const std::vector<range_case> range_cases = {
    {"coin", {UINT32_C(5), UINT32_C(2), UINT32_C(7)}, 3, INT64_C(0), INT64_C(1), 3, {INT64_C(1), INT64_C(0), INT64_C(1)}},
    {"no-results", {}, 0, INT64_C(1), INT64_C(6), 0, {}},
    {"die", {UINT32_C(7), UINT32_C(12), UINT32_C(1000000), UINT32_C(4294967295)}, 4, INT64_C(1), INT64_C(6), 4, {INT64_C(2), INT64_C(1), INT64_C(5), INT64_C(4)}},
    {"negative-range", {UINT32_C(100), UINT32_C(2000000000), UINT32_C(4294967294)}, 3, INT64_C(-10), INT64_C(10), 3, {INT64_C(6), INT64_C(-5), INT64_C(-8)}},
    {"one-value", {UINT32_C(0), UINT32_C(99)}, 2, INT64_C(5), INT64_C(5), 2, {INT64_C(5), INT64_C(5)}},
    {"reject-low-words", {UINT32_C(0), UINT32_C(3), UINT32_C(4)}, 3, INT64_C(1), INT64_C(6), 1, {INT64_C(5)}},
    {"accept-least", {UINT32_C(3), UINT32_C(4), UINT32_C(9)}, 3, INT64_C(0), INT64_C(5), 2, {INT64_C(4), INT64_C(3)}},
    {"power-of-two", {UINT32_C(0), UINT32_C(4294967295)}, 2, INT64_C(0), INT64_C(7), 2, {INT64_C(0), INT64_C(7)}},
    {"worst-case", {UINT32_C(2147483646), UINT32_C(2147483647)}, 2, INT64_C(0), INT64_C(2147483648), 1, {INT64_C(2147483647)}},
    {"full-unsigned-32", {UINT32_C(0), UINT32_C(4294967295), UINT32_C(123456789)}, 3, INT64_C(0), INT64_C(4294967295), 3, {INT64_C(0), INT64_C(4294967295), INT64_C(123456789)}},
    {"full-signed-32", {UINT32_C(0), UINT32_C(2147483648), UINT32_C(4294967295)}, 3, INT64_C(-2147483648), INT64_C(2147483647), 3, {INT64_C(-2147483648), INT64_C(0), INT64_C(2147483647)}},
    {"near-int64-max", {UINT32_C(4294967295), UINT32_C(6)}, 2, INT64_C(9223372036854775798), INT64_C(9223372036854775807), 2, {INT64_C(9223372036854775803), INT64_C(9223372036854775804)}},
    {"near-int64-min", {UINT32_C(4294967295), UINT32_C(9)}, 2, INT64_MIN, INT64_C(-9223372036854775799), 2, {INT64_C(-9223372036854775803), INT64_C(-9223372036854775799)}},
    {"wide-int64", {UINT32_C(4294967295), UINT32_C(0)}, 2, INT64_C(9223372032559808512), INT64_C(9223372036854775807), 2, {INT64_C(9223372036854775807), INT64_C(9223372032559808512)}},
};

struct shuffle_case {
    const char* name;
    std::vector<std::uint32_t> words;
    std::size_t nwords;
    std::vector<std::string> items, want;
    std::size_t count;
};

const std::vector<shuffle_case> shuffle_cases = {
    {"empty", {}, 0, {}, {}, 0},
    {"one-item", {}, 0, {"a"}, {"a"}, 1},
    {"two-items-swap", {UINT32_C(0)}, 1, {"a", "b"}, {"b", "a"}, 2},
    {"two-items-keep", {UINT32_C(1)}, 1, {"a", "b"}, {"a", "b"}, 2},
    {"five-items", {UINT32_C(7), UINT32_C(3), UINT32_C(4), UINT32_C(1)}, 4, {"a", "b", "c", "d", "e"}, {"a", "e", "b", "d", "c"}, 5},
    {"reject-in-shuffle", {UINT32_C(0), UINT32_C(2), UINT32_C(0)}, 3, {"a", "b", "c"}, {"b", "a", "c"}, 3},
    {"duplicates", {UINT32_C(1), UINT32_C(1)}, 2, {"x", "x", "y"}, {"x", "y", "x"}, 3},
    {"non-ascii", {UINT32_C(4294967295), UINT32_C(4294967294), UINT32_C(0)}, 3, {"é", "日本", "🙂", "a"}, {"日本", "é", "🙂", "a"}, 4},
};

// A word source that gives the words in order.
struct fake_words {
    const std::vector<std::uint32_t>& words;
    std::size_t next = 0;

    std::uint32_t operator()() { return words.at(next++); }
};

int failures = 0;

void check(bool ok, const std::string& name, const char* what) {
    if (!ok) {
        std::printf("FAIL %s: %s\n", name.c_str(), what);
        ++failures;
    }
}

template <typename F>
bool throws(F f) {
    try {
        f();
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

}  // namespace

int main() {
    using namespace random_numbers;
    for (const auto& c : range_cases) {
        fake_words f{c.words};
        std::vector<std::int64_t> got;
        for (std::size_t k = 0; k < c.count; ++k) {
            got.push_back(int_in_range(f, c.lo, c.hi));
        }
        check(got == c.want, c.name, "wrong values");
        check(f.next == c.words.size(), c.name, "did not use all words");
    }
    for (const auto& c : shuffle_cases) {
        fake_words f{c.words};
        std::vector<std::string> items = c.items;
        shuffle(f, std::span{items});
        check(items == c.want, c.name, "wrong order");
        check(f.next == c.words.size(), c.name, "did not use all words");
    }
    auto zero = [] { return std::uint32_t{0}; };
    check(throws([&] { int_in_range(zero, 1, 0); }), "bad-range", "hi < lo");
    check(throws([&] { int_in_range(zero, 0, std::int64_t{1} << 32); }), "bad-range", "2^32 + 1 values");
    check(throws([&] { int_in_range(zero, INT64_MIN, INT64_MAX); }), "bad-range", "2^64 values");

    std::mt19937 a(42), b(42), c(43);
    bool same = true, other = true;
    for (int k = 0; k < 10; ++k) {
        const auto wa = a();
        same = same && wa == b();
        other = other && wa == c();
    }
    check(same, "same-seed", "different words");
    check(!other, "other-seed", "same words");

    for (int k = 0; k < 100; ++k) {
        const auto v = int_in_range(secure_word, 1, 6);
        check(v >= 1 && v <= 6, "secure-words", "out of range");
    }
    for (std::size_t n : {0, 1, 16, 32, 300}) {
        const std::string token = token_hex(n);
        check(std::regex_match(token, std::regex("[0-9a-f]*")) && token.size() == 2 * n, "token-hex", "wrong digits");
    }
    check(token_hex(16) != token_hex(16), "tokens-differ", "equal");

    if (failures > 0) {
        std::printf("%d failures\n", failures);
        return 1;
    }
    std::printf("ok\n");
    return 0;
}

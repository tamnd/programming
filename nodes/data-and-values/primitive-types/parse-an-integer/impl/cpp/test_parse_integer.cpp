// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

#include "parse_integer.hpp"

namespace {

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
        std::string_view name;
        std::string s;
        int base;
        std::int64_t want;
    };
    const std::vector<Case> cases = {
        {"decimal", "42", 10, 42},
        {"negative", "-17", 10, -17},
        {"hex", "ff", 16, 255},
        {"binary", "101010", 2, 42},
        {"base-36", "zz", 36, 1295},
        {"plus-sign", "+5", 10, 5},
        {"negative-zero", "-0", 10, 0},
        {"zero", "0", 10, 0},
        {"leading-zeros", "007", 10, 7},
        {"many-leading-zeros", std::string(5000, '0') + "42", 10, 42},
        {"max", "9223372036854775807", 10, 9223372036854775807LL},
        {"min", "-9223372036854775808", 10, std::numeric_limits<std::int64_t>::min()},
        {"max-hex", "7fffffffffffffff", 16, 9223372036854775807LL},
        {"min-binary", "-1000000000000000000000000000000000000000000000000000000000000000", 2, std::numeric_limits<std::int64_t>::min()},
        {"upper-case", "FF", 16, 255},
        {"mixed-case", "Zz", 36, 1295},
        {"above-safe-integer", "9007199254740993", 10, 9007199254740993LL},
    };
    for (const auto &c : cases) {
        auto got = parse_integer(c.s, c.base);
        check(c.name, got.has_value() && *got == c.want);
    }

    struct ErrorCase {
        std::string_view name;
        std::string s;
        int base;
        std::errc want;
    };
    const std::vector<ErrorCase> errors = {
        {"empty", "", 10, std::errc::invalid_argument},
        {"sign-only", "-", 10, std::errc::invalid_argument},
        {"plus-only", "+", 10, std::errc::invalid_argument},
        {"double-sign", "+-5", 10, std::errc::invalid_argument},
        {"leading-space", " 42", 10, std::errc::invalid_argument},
        {"trailing-space", "42 ", 10, std::errc::invalid_argument},
        {"trailing-newline", "42\n", 10, std::errc::invalid_argument},
        {"trailing-garbage", "42px", 10, std::errc::invalid_argument},
        {"underscore", "1_000", 10, std::errc::invalid_argument},
        {"thousands-separator", "1,000", 10, std::errc::invalid_argument},
        {"decimal-point", "4.0", 10, std::errc::invalid_argument},
        {"exponent", "1e3", 10, std::errc::invalid_argument},
        {"hex-prefix", "0x1A", 16, std::errc::invalid_argument},
        {"binary-prefix", "0b101", 2, std::errc::invalid_argument},
        {"digit-outside-base", "102", 2, std::errc::invalid_argument},
        {"letter-outside-base", "fg", 16, std::errc::invalid_argument},
        {"arabic-indic-digits", "\xd9\xa1\xd9\xa2", 10, std::errc::invalid_argument},
        {"fullwidth-digits", "\xef\xbc\x91\xef\xbc\x92", 10, std::errc::invalid_argument},
        {"kelvin-sign", "\xe2\x84\xaa", 36, std::errc::invalid_argument},
        {"max-plus-one", "9223372036854775808", 10, std::errc::result_out_of_range},
        {"min-minus-one", "-9223372036854775809", 10, std::errc::result_out_of_range},
        {"two-to-the-64", "18446744073709551616", 10, std::errc::result_out_of_range},
        {"hex-sign-bit", "8000000000000000", 16, std::errc::result_out_of_range},
        {"hex-all-ones", "ffffffffffffffff", 16, std::errc::result_out_of_range},
        {"min-times-ten", "-92233720368547758080", 10, std::errc::result_out_of_range},
        {"many-digits", std::string(5000, '9'), 10, std::errc::result_out_of_range},
    };
    for (const auto &c : errors) {
        auto got = parse_integer(c.s, c.base);
        check(c.name, !got.has_value() && got.error() == c.want);
    }

    if (failures > 0) {
        std::fprintf(stderr, "%d cases failed\n", failures);
        return 1;
    }
    std::printf("all cases passed\n");
    return 0;
}

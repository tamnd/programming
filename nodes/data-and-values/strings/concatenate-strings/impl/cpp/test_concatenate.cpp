// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>
#include <string_view>

#include "concatenate.hpp"

namespace {

int failures = 0;

void check(std::string_view name, const std::string& got, std::string_view want) {
    if (got != want) {
        std::fprintf(stderr, "case %.*s failed\n", static_cast<int>(name.size()), name.data());
        ++failures;
    }
}

}  // namespace

int main() {
    check("hello-world", concat("Hello, ", "world"), "Hello, world");
    check("two-words", concat("good", "bye"), "goodbye");
    check("keep-spaces", concat("a ", " b"), "a  b");
    check("empty-both", concat("", ""), "");
    check("empty-left", concat("", "abc"), "abc");
    check("empty-right", concat("abc", ""), "abc");
    check("non-ascii", concat("naï", "ve"), "naïve");
    check("astral", concat("a", "🙂"), "a🙂");
    check("combining-mark", concat("e", "\xcc\x81"), "e\xcc\x81");
    check("flag-halves", concat("🇫", "🇷"), "🇫🇷");
    check("digit-strings", concat("1", "2"), "12");

    check("positive", concat_number("item", 3), "item3");
    check("zero", concat_number("n=", 0), "n=0");
    check("negative", concat_number("x", -5), "x-5");
    check("empty-prefix", concat_number("", 42), "42");
    check("int32-max", concat_number("max=", std::numeric_limits<std::int32_t>::max()), "max=2147483647");
    check("int32-min", concat_number("min=", std::numeric_limits<std::int32_t>::min()), "min=-2147483648");
    return failures == 0 ? 0 : 1;
}

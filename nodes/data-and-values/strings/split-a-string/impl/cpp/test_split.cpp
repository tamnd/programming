// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

#include <cstdio>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

#include "split.hpp"

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
    using List = std::vector<std::string_view>;
    struct Case {
        std::string_view name, s, sep;
        std::size_t limit;
        List want;
    };
    const std::vector<Case> cases = {
        {"comma", "a,b,c", ",", 0, {"a", "b", "c"}},
        {"csv-line", "Ada,1815,London", ",", 0, {"Ada", "1815", "London"}},
        {"multi-char-separator", "a::b::c", "::", 0, {"a", "b", "c"}},
        {"space-separator", "one two  three", " ", 0, {"one", "two", "", "three"}},
        {"limit-two", "key=value=more", "=", 2, {"key", "value=more"}},
        {"empty", "", ",", 0, {""}},
        {"no-separator", "abc", ",", 0, {"abc"}},
        {"leading-separator", ",a,b", ",", 0, {"", "a", "b"}},
        {"trailing-separators", "a,b,,", ",", 0, {"a", "b", "", ""}},
        {"adjacent-separators", "a,,b", ",", 0, {"a", "", "b"}},
        {"only-separator", ",", ",", 0, {"", ""}},
        {"overlapping-match", "aaa", "aa", 0, {"", "a"}},
        {"dot-separator", "192.168.0.1", ".", 0, {"192", "168", "0", "1"}},
        {"pipe-separator", "a|b|c", "|", 0, {"a", "b", "c"}},
        {"non-ascii", "día→naïve→日本→🙂", "→", 0, {"día", "naïve", "日本", "🙂"}},
        {"limit-one", "a,b,c", ",", 1, {"a,b,c"}},
        {"limit-above-count", "a,b", ",", 5, {"a", "b"}},
        {"limit-keeps-trailing-empty", "a,b,", ",", 3, {"a", "b", ""}},
    };
    for (const auto& c : cases) {
        check(c.name, split(c.s, c.sep, c.limit) == c.want);
    }

    struct WhitespaceCase {
        std::string_view name, s;
        List want;
    };
    const std::vector<WhitespaceCase> whitespace_cases = {
        {"single-spaces", "a b c", {"a", "b", "c"}},
        {"runs-and-ends", "  lead  and trail  ", {"lead", "and", "trail"}},
        {"tabs-and-newlines", "a\t\tb\nc\n", {"a", "b", "c"}},
        {"one-word", "word", {"word"}},
        {"empty", "", {}},
        {"only-white-space", " \t\n ", {}},
    };
    for (const auto& c : whitespace_cases) {
        check(c.name, split_whitespace(c.s) == c.want);
    }

    struct RegexCase {
        std::string_view name;
        std::string s, pattern;
        std::vector<std::string> want;
    };
    const std::vector<RegexCase> regex_cases = {
        {"comma-or-semicolon", "a, b;c ,d", "[ ]*[,;][ ]*", {"a", "b", "c", "d"}},
        {"digit-runs", "a1b22c333", "[0-9]+", {"a", "b", "c", ""}},
        {"adjacent-matches", "a,;b", "[,;]", {"a", "", "b"}},
        {"leading-and-trailing", ";a;", ";", {"", "a", ""}},
        {"empty", "", "[,;]", {""}},
        {"no-match", "abc", "[0-9]+", {"abc"}},
    };
    for (const auto& c : regex_cases) {
        check(c.name, split_regex(c.s, c.pattern) == c.want);
    }

    bool threw = false;
    try {
        split_regex("a", "[a");
    } catch (const std::regex_error&) {
        threw = true;
    }
    check("bad-pattern", threw);
    return failures == 0 ? 0 : 1;
}

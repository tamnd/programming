// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

#include "unique.hpp"

#include <cstdio>
#include <string>
#include <vector>

namespace {

using Strings = std::vector<std::string>;
using Users = std::vector<dedupe::User>;

int failures = 0;

void report(const std::string& name, bool ok) {
    std::printf("%s %s\n", ok ? "ok" : "FAIL", name.c_str());
    if (!ok) ++failures;
}

void check(const char* name, const Strings& given, const Strings& want) {
    Strings input = given;
    report(name, dedupe::unique(input) == want && input == given);
}

void check_users(const char* name, const Users& given, const Users& want) {
    report(std::string("by-key ") + name, dedupe::unique_by_email(given) == want);
}

}  // namespace

int main() {
    check("repeats", {"apple", "pear", "apple", "fig", "pear"}, {"apple", "pear", "fig"});
    check("words", {"to", "be", "or", "not", "to", "be"}, {"to", "be", "or", "not"});
    check("empty", {}, {});
    check("single", {"a"}, {"a"});
    check("all-same", {"x", "x", "x"}, {"x"});
    check("no-duplicates", {"c", "a", "b"}, {"c", "a", "b"});
    check("not-adjacent", {"a", "b", "a", "b"}, {"a", "b"});
    check("adjacent", {"a", "a", "b", "b", "a"}, {"a", "b"});
    check("empty-string", {"", "a", "", "b"}, {"", "a", "b"});
    check("case", {"a", "A", "a"}, {"a", "A"});
    check("normalization", {"\u00e9", "e\u0301", "\u00e9"}, {"\u00e9", "e\u0301"});
    check("whitespace", {" a", "a", "a ", " a"}, {" a", "a", "a "});
    check("number-like", {"1", "01", "1.0", "1"}, {"1", "01", "1.0"});
    check("non-ascii", {"日本", "🙂", "日本", "🙂", "ü"}, {"日本", "🙂", "ü"});

    // The cases of cases/by-key.jsonl.
    check_users("case-insensitive",
                {{"Ada", "ada@example.com"}, {"Bob", "bob@example.com"}, {"Ada L.", "ADA@Example.com"}},
                {{"Ada", "ada@example.com"}, {"Bob", "bob@example.com"}});
    check_users("empty", {}, {});
    check_users("same-name-other-email", {{"Sam", "sam@a.example"}, {"Sam", "sam@b.example"}},
                {{"Sam", "sam@a.example"}, {"Sam", "sam@b.example"}});
    check_users("first-record-wins", {{"Old", "x@example.com"}, {"New", "X@EXAMPLE.COM"}},
                {{"Old", "x@example.com"}});
    check_users("dots-differ", {{"Al", "a.b@example.com"}, {"Bo", "ab@example.com"}},
                {{"Al", "a.b@example.com"}, {"Bo", "ab@example.com"}});
    check_users("not-adjacent",
                {{"A", "a@example.com"}, {"B", "b@example.com"}, {"C", "A@example.com"}, {"D", "B@EXAMPLE.com"}},
                {{"A", "a@example.com"}, {"B", "b@example.com"}});
    return failures == 0 ? 0 : 1;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

#include "unique.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void report(const char *name, bool ok) {
    printf("%s %s\n", ok ? "ok" : "FAIL", name);
    if (!ok) {
        failures++;
    }
}

static void check(const char *name, const char **given, size_t n, const char *const *want, size_t m) {
    size_t k = unique(given, n);
    bool ok = k == m;
    for (size_t i = 0; ok && i < m; i++) {
        ok = strcmp(given[i], want[i]) == 0;
    }
    report(name, ok);
}

static void check_users(const char *name, struct user *given, size_t n, const struct user *want, size_t m) {
    size_t k = unique_by_email(given, n);
    bool ok = k == m;
    for (size_t i = 0; ok && i < m; i++) {
        ok = strcmp(given[i].name, want[i].name) == 0 && strcmp(given[i].email, want[i].email) == 0;
    }
    report(name, ok);
}

#define L(...) {__VA_ARGS__}

#define CHECK(name, given, ...)                                      \
    do {                                                             \
        const char *g[] = given;                                     \
        static const char *const w[] = __VA_ARGS__;                  \
        check(name, g, sizeof g / sizeof g[0], w, sizeof w / sizeof w[0]); \
    } while (0)

#define CHECK_USERS(name, given, ...)                                \
    do {                                                             \
        struct user g[] = given;                                     \
        static const struct user w[] = __VA_ARGS__;                  \
        check_users(name, g, sizeof g / sizeof g[0], w, sizeof w / sizeof w[0]); \
    } while (0)

int main(void) {
    CHECK("repeats", L("apple", "pear", "apple", "fig", "pear"), {"apple", "pear", "fig"});
    CHECK("words", L("to", "be", "or", "not", "to", "be"), {"to", "be", "or", "not"});
    report("empty", unique(NULL, 0) == 0);
    CHECK("single", L("a"), {"a"});
    CHECK("all-same", L("x", "x", "x"), {"x"});
    CHECK("no-duplicates", L("c", "a", "b"), {"c", "a", "b"});
    CHECK("not-adjacent", L("a", "b", "a", "b"), {"a", "b"});
    CHECK("adjacent", L("a", "a", "b", "b", "a"), {"a", "b"});
    CHECK("empty-string", L("", "a", "", "b"), {"", "a", "b"});
    CHECK("case", L("a", "A", "a"), {"a", "A"});
    CHECK("normalization", L("\u00e9", "e\u0301", "\u00e9"), {"\u00e9", "e\u0301"});
    CHECK("whitespace", L(" a", "a", "a ", " a"), {" a", "a", "a "});
    CHECK("number-like", L("1", "01", "1.0", "1"), {"1", "01", "1.0"});
    CHECK("non-ascii", L("日本", "🙂", "日本", "🙂", "ü"), {"日本", "🙂", "ü"});

    // The cases of cases/by-key.jsonl.
    CHECK_USERS("by-key case-insensitive",
                L({"Ada", "ada@example.com"}, {"Bob", "bob@example.com"}, {"Ada L.", "ADA@Example.com"}),
                {{"Ada", "ada@example.com"}, {"Bob", "bob@example.com"}});
    report("by-key empty", unique_by_email(NULL, 0) == 0);
    CHECK_USERS("by-key same-name-other-email", L({"Sam", "sam@a.example"}, {"Sam", "sam@b.example"}),
                {{"Sam", "sam@a.example"}, {"Sam", "sam@b.example"}});
    CHECK_USERS("by-key first-record-wins", L({"Old", "x@example.com"}, {"New", "X@EXAMPLE.COM"}),
                {{"Old", "x@example.com"}});
    CHECK_USERS("by-key dots-differ", L({"Al", "a.b@example.com"}, {"Bo", "ab@example.com"}),
                {{"Al", "a.b@example.com"}, {"Bo", "ab@example.com"}});
    CHECK_USERS("by-key not-adjacent",
                L({"A", "a@example.com"}, {"B", "b@example.com"}, {"C", "A@example.com"}, {"D", "B@EXAMPLE.com"}),
                {{"A", "a@example.com"}, {"B", "b@example.com"}});
    return failures == 0 ? 0 : 1;
}

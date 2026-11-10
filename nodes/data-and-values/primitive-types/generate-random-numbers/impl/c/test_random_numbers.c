// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

#include "random_numbers.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

struct range_case {
    const char *name;
    uint32_t words[8];
    size_t nwords;
    int64_t lo, hi;
    size_t count;
    int64_t want[8];
};

static const struct range_case range_cases[] = {
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
    const char *name;
    uint32_t words[8];
    size_t nwords;
    const char *items[8];
    const char *want[8];
    size_t count;
};

static const struct shuffle_case shuffle_cases[] = {
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
    const uint32_t *words;
    size_t len, next;
};

static uint32_t fake_word(void *ctx) {
    struct fake_words *f = ctx;
    if (f->next >= f->len) {
        fprintf(stderr, "no more words\n");
        return 0;
    }
    return f->words[f->next++];
}

static int failures;

static void check(bool ok, const char *name, const char *what) {
    if (!ok) {
        printf("FAIL %s: %s\n", name, what);
        failures++;
    }
}

static void test_ranges(void) {
    for (size_t i = 0; i < sizeof range_cases / sizeof range_cases[0]; i++) {
        const struct range_case *c = &range_cases[i];
        struct fake_words f = {c->words, c->nwords, 0};
        for (size_t k = 0; k < c->count; k++) {
            int64_t got;
            check(int_in_range(fake_word, &f, c->lo, c->hi, &got), c->name, "returned false");
            check(got == c->want[k], c->name, "wrong value");
        }
        check(f.next == f.len, c->name, "did not use all words");
    }
}

static void test_shuffles(void) {
    for (size_t i = 0; i < sizeof shuffle_cases / sizeof shuffle_cases[0]; i++) {
        const struct shuffle_case *c = &shuffle_cases[i];
        struct fake_words f = {c->words, c->nwords, 0};
        const char *items[8];
        memcpy(items, c->items, sizeof items);
        check(shuffle(fake_word, &f, items, c->count, sizeof items[0]), c->name, "returned false");
        for (size_t k = 0; k < c->count; k++) {
            check(strcmp(items[k], c->want[k]) == 0, c->name, "wrong order");
        }
        check(f.next == f.len, c->name, "did not use all words");
    }
}

static void test_bad_ranges(void) {
    int64_t out;
    check(!int_in_range(secure_word, nullptr, 1, 0, &out), "bad-range", "hi < lo");
    check(!int_in_range(secure_word, nullptr, 0, INT64_C(1) << 32, &out), "bad-range", "2^32 + 1 values");
    check(!int_in_range(secure_word, nullptr, INT64_MIN, INT64_MAX, &out), "bad-range", "2^64 values");
}

static void test_splitmix64(void) {
    static const uint32_t want[] = {3184996902, 686809907, 1196582743, 1478287871, 163338330};
    struct splitmix64 g = {42};
    for (size_t k = 0; k < 5; k++) {
        check(splitmix64_word(&g) == want[k], "splitmix64", "differs from the reference code");
    }
    struct splitmix64 a = {7}, b = {7}, c = {8};
    bool same = true, other = true;
    for (int k = 0; k < 10; k++) {
        uint32_t wa = splitmix64_word(&a);
        same = same && wa == splitmix64_word(&b);
        other = other && wa == splitmix64_word(&c);
    }
    check(same, "same-seed", "different words");
    check(!other, "other-seed", "same words");
}

static void test_secure(void) {
    for (int k = 0; k < 100; k++) {
        int64_t v;
        check(int_in_range(secure_word, nullptr, 1, 6, &v) && v >= 1 && v <= 6, "secure-words", "out of range");
    }
    static const size_t sizes[] = {0, 1, 16, 32, 300};
    for (size_t i = 0; i < sizeof sizes / sizeof sizes[0]; i++) {
        char buf[601];
        check(token_hex(sizes[i], buf, sizeof buf), "token-hex", "returned false");
        check(strlen(buf) == 2 * sizes[i], "token-hex", "wrong length");
        check(strspn(buf, "0123456789abcdef") == 2 * sizes[i], "token-hex", "not lowercase hex");
    }
    char a[33], b[33], small[32];
    check(token_hex(16, a, sizeof a) && token_hex(16, b, sizeof b) && strcmp(a, b) != 0, "tokens-differ", "equal");
    check(!token_hex(16, small, sizeof small), "token-hex-small", "returned true");
}

int main(void) {
    test_ranges();
    test_shuffles();
    test_bad_ranges();
    test_splitmix64();
    test_secure();
    if (failures > 0) {
        printf("%d failures\n", failures);
        return 1;
    }
    printf("ok\n");
    return 0;
}

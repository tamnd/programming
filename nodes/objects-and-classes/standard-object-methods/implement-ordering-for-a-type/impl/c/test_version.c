// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

#include "version.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char **versions, const char *const *want, size_t n) {
    if (sort_versions(versions, n) != 0) {
        fprintf(stderr, "case %s failed: error\n", name);
        failures++;
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (strcmp(versions[i], want[i]) != 0) {
            fprintf(stderr, "case %s failed at %zu: got %s, want %s\n", name, i, versions[i], want[i]);
            failures++;
            return;
        }
    }
}

static void expect(const char *name, bool ok) {
    if (!ok) {
        fprintf(stderr, "check %s failed\n", name);
        failures++;
    }
}

#define CASE(name, n, in, want) check(name, (const char *[])in, (const char *const[])want, n)
#define L(...) {__VA_ARGS__}

int main(void) {
    CASE("simple", 3, L("1.2.0", "1.0.0", "1.1.0"), L("1.0.0", "1.1.0", "1.2.0"));
    CASE("fields-in-order", 3, L("2.0.0", "1.9.9", "1.10.0"), L("1.9.9", "1.10.0", "2.0.0"));
    CASE("patch", 3, L("0.1.10", "0.1.2", "0.1.1"), L("0.1.1", "0.1.2", "0.1.10"));
    check("empty", NULL, NULL, 0);
    CASE("single", 1, L("3.4.5"), L("3.4.5"));
    CASE("numeric-not-text", 3, L("10.0.0", "9.0.0", "1.0.0"), L("1.0.0", "9.0.0", "10.0.0"));
    CASE("duplicates", 3, L("1.0.0", "0.9.0", "1.0.0"), L("0.9.0", "1.0.0", "1.0.0"));
    CASE("zero", 2, L("0.0.1", "0.0.0"), L("0.0.0", "0.0.1"));
    CASE("limit", 2, L("1000000.0.0", "999999.1000000.1000000"), L("999999.1000000.1000000", "1000000.0.0"));

    Version a = {1, 2, 3}, b = {1, 10, 0}, c = {1, 2, 3};
    expect("less", version_compare(&a, &b) < 0 && version_compare(&b, &a) > 0);
    expect("equal", version_compare(&a, &c) == 0);
    Version v;
    expect("parse", version_parse("1.10.0", &v) && v.major == 1 && v.minor == 10 && v.patch == 0);
    expect("parse two parts", !version_parse("1.2", &v));
    expect("parse sign", !version_parse("1.-2.0", &v));
    const char *bad[] = {"1.0.0", "x"};
    expect("bad input", sort_versions(bad, 2) == -1 && strcmp(bad[0], "1.0.0") == 0);

    if (failures == 0) {
        printf("all cases passed\n");
    }
    return failures == 0 ? 0 : 1;
}

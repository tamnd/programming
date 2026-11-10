// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

#include "sort_employees.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static int same(const struct employee *a, const struct employee *b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (strcmp(a[i].name, b[i].name) != 0 || strcmp(a[i].dept, b[i].dept) != 0 ||
            a[i].salary != b[i].salary) {
            return 0;
        }
    }
    return 1;
}

static void check(const char *name, struct employee *given, const struct employee *want, size_t n) {
    if (sort_employees(given, n) != 0 || !same(given, want, n)) {
        printf("FAIL %s\n", name);
        failures++;
    } else {
        printf("ok %s\n", name);
    }
}

#define CHECK(name, given, ...)                                         \
    do {                                                                \
        struct employee g[] = given;                                    \
        static const struct employee w[] = __VA_ARGS__;                 \
        _Static_assert(sizeof g == sizeof w, "the same length");        \
        check(name, g, w, sizeof g / sizeof g[0]);                      \
    } while (0)

#define L(...) {__VA_ARGS__}

static const char *const names[40] = {
    "n01", "n02", "n03", "n04", "n05", "n06", "n07", "n08", "n09", "n10",
    "n11", "n12", "n13", "n14", "n15", "n16", "n17", "n18", "n19", "n20",
    "n21", "n22", "n23", "n24", "n25", "n26", "n27", "n28", "n29", "n30",
    "n31", "n32", "n33", "n34", "n35", "n36", "n37", "n38", "n39", "n40",
};

int main(void) {
    CHECK("mixed",
          L({"Ada", "eng", 120}, {"Bob", "sales", 90}, {"Cy", "eng", 150}, {"Di", "ops", 100}),
          {{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Di", "ops", 100}, {"Bob", "sales", 90}});
    CHECK("same-salary-other-dept", L({"Ann", "ops", 100}, {"Ben", "eng", 100}, {"Cat", "hr", 100}),
          {{"Ben", "eng", 100}, {"Cat", "hr", 100}, {"Ann", "ops", 100}});
    if (sort_employees(NULL, 0) != 0) {
        printf("FAIL empty\n");
        failures++;
    } else {
        printf("ok empty\n");
    }
    CHECK("single", L({"Ada", "eng", 120}), {{"Ada", "eng", 120}});
    CHECK("sorted", L({"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}),
          {{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}});
    CHECK("reversed", L({"Bob", "ops", 90}, {"Ada", "eng", 120}, {"Cy", "eng", 150}),
          {{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}});
    CHECK("ties",
          L({"Zoe", "eng", 100}, {"Xi", "hr", 80}, {"Al", "eng", 100}, {"Yu", "hr", 80}, {"Mo", "eng", 100}),
          {{"Zoe", "eng", 100}, {"Al", "eng", 100}, {"Mo", "eng", 100}, {"Xi", "hr", 80}, {"Yu", "hr", 80}});

    // The 40 records of the many-ties case: depts "b" and "a" in turn, all with salary 1.
    struct employee given[40], want[40];
    for (size_t i = 0; i < 40; i++) {
        given[i] = (struct employee){names[i], i % 2 ? "a" : "b", 1};
    }
    for (size_t i = 0; i < 20; i++) {
        want[i] = given[2 * i + 1];
        want[20 + i] = given[2 * i];
    }
    check("many-ties", given, want, 40);

    CHECK("case", L({"Ann", "eng", 100}, {"Bea", "Sales", 90}, {"Cal", "ops", 80}),
          {{"Bea", "Sales", 90}, {"Ann", "eng", 100}, {"Cal", "ops", 80}});
    CHECK("non-ascii", L({"Eve", "équipe", 100}, {"Fay", "zoo", 90}, {"Gus", "eng", 80}),
          {{"Gus", "eng", 80}, {"Fay", "zoo", 90}, {"Eve", "équipe", 100}});
    CHECK("negative", L({"Hal", "eng", 0}, {"Ida", "eng", -5}, {"Jo", "eng", 5}),
          {{"Jo", "eng", 5}, {"Hal", "eng", 0}, {"Ida", "eng", -5}});
    CHECK("overflow", L({"Kim", "eng", -2000000000}, {"Lee", "eng", 2000000000}),
          {{"Lee", "eng", 2000000000}, {"Kim", "eng", -2000000000}});
    return failures == 0 ? 0 : 1;
}

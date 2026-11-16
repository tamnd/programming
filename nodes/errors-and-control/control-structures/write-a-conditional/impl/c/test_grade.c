// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

#include "grade.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int score, const char *want) {
    if (strcmp(grade(score), want) != 0) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("ninety-five", 95, "A");
    check("eighty-five", 85, "B");
    check("seventy-five", 75, "C");
    check("sixty-five", 65, "D");
    check("forty", 40, "F");
    check("hundred", 100, "A");
    check("zero", 0, "F");
    check("ninety", 90, "A");
    check("eighty-nine", 89, "B");
    check("sixty", 60, "D");
    check("fifty-nine", 59, "F");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}

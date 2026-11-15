// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

#include "fizzbuzz.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int n, const char *const *want) {
    char **out = fizzbuzz(n);
    int ok = out != NULL;
    for (int i = 0; ok && i < n; i++) {
        ok = strcmp(out[i], want[i]) == 0;
    }
    fizzbuzz_free(out, n);
    if (!ok) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

int main(void) {
    const char *thirty[] = {"1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz", "16", "17", "Fizz", "19", "Buzz", "Fizz", "22", "23", "Fizz", "Buzz", "26", "Fizz", "28", "29", "FizzBuzz"};
    check("fifteen", 15, thirty);
    check("thirty", 30, thirty);
    check("zero", 0, thirty);
    check("one", 1, thirty);
    check("three", 3, thirty);
    check("five", 5, thirty);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}

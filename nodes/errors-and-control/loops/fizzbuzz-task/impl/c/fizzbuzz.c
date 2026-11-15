// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

#include "fizzbuzz.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fizzbuzz_free(char **out, int n) {
    if (out == NULL) {
        return;
    }
    for (int i = 0; i < n; i++) {
        free(out[i]);
    }
    free(out);
}

char **fizzbuzz(int n) {
    // malloc(0) may return NULL, so ask for at least one pointer.
    char **out = malloc((size_t)(n > 0 ? n : 1) * sizeof *out);
    if (out == NULL) {
        return NULL;
    }
    for (int i = 1; i <= n; i++) {
        // 12 bytes hold "FizzBuzz" or any int in decimal, with the 0.
        char *s = malloc(12);
        if (s == NULL) {
            fizzbuzz_free(out, i - 1);
            return NULL;
        }
        // Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
        if (i % 15 == 0) {
            strcpy(s, "FizzBuzz");
        } else if (i % 3 == 0) {
            strcpy(s, "Fizz");
        } else if (i % 5 == 0) {
            strcpy(s, "Buzz");
        } else {
            snprintf(s, 12, "%d", i);
        }
        out[i - 1] = s;
    }
    return out;
}

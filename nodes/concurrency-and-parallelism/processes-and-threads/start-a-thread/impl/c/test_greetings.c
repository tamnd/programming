// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

#include "greetings.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *const names[], const char *const want[], size_t n) {
    char **got = thread_greetings(names, n);
    if (got == NULL) {
        fprintf(stderr, "case %s failed: NULL\n", name);
        failures++;
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (strcmp(got[i], want[i]) != 0) {
            fprintf(stderr, "case %s failed at %zu: got \"%s\", want \"%s\"\n", name, i, got[i], want[i]);
            failures++;
            break;
        }
    }
    free_greetings(got, n);
}

int main(void) {
    check("two", (const char *[]){"Ada", "Linus"}, (const char *[]){"hello from Ada", "hello from Linus"}, 2);
    check("three", (const char *[]){"main", "worker", "logger"},
          (const char *[]){"hello from main", "hello from worker", "hello from logger"}, 3);
    check("empty", NULL, NULL, 0);
    check("single", (const char *[]){"solo"}, (const char *[]){"hello from solo"}, 1);
    check("empty-name", (const char *[]){""}, (const char *[]){"hello from "}, 1);
    check("duplicate", (const char *[]){"twin", "twin"}, (const char *[]){"hello from twin", "hello from twin"}, 2);
    check("unicode", (const char *[]){"Zoë", "李雷"}, (const char *[]){"hello from Zoë", "hello from 李雷"}, 2);

    char names[32][8], wants[32][24];
    const char *many[32], *many_want[32];
    for (int i = 0; i < 32; i++) {
        snprintf(names[i], sizeof names[i], "t%d", i);
        snprintf(wants[i], sizeof wants[i], "hello from t%d", i);
        many[i] = names[i];
        many_want[i] = wants[i];
    }
    check("many", many, many_want, 32);

    if (failures == 0) {
        printf("all tests passed\n");
    }
    return failures == 0 ? 0 : 1;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

// setenv and unsetenv are POSIX, not ISO C.
#define _POSIX_C_SOURCE 200809L

#include "get_env.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

// check sets the variable key to value (no variable when key is NULL), unsets
// name if it is a different variable, and compares get_env(name, def) with want.
static void check(const char *label, const char *key, const char *value,
                  const char *name, const char *def, const char *want) {
    unsetenv(name);
    if (key != NULL && setenv(key, value, 1) != 0) {
        perror("setenv");
        exit(1);
    }
    const char *got = get_env(name, def);
    if (strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got \"%s\", want \"%s\"\n", label, got, want);
        failures++;
    }
    if (key != NULL) {
        unsetenv(key);
    }
}

int main(void) {
    check("present", "APP_PORT", "8080", "APP_PORT", "80", "8080");
    check("path-value", "APP_HOME", "/srv/app", "APP_HOME", "/opt/app", "/srv/app");
    check("missing", NULL, NULL, "APP_PORT", "80", "80");
    check("empty-value", "APP_MODE", "", "APP_MODE", "production", "");
    check("empty-default", NULL, NULL, "APP_MODE", "", "");
    check("spaces", "APP_TITLE", "  two  spaces  ", "APP_TITLE", "none", "  two  spaces  ");
    check("equals", "APP_QUERY", "key=value=more", "APP_QUERY", "none", "key=value=more");
    check("newline", "APP_TEXT", "first\nsecond", "APP_TEXT", "none", "first\nsecond");
    check("non-ascii", "APP_GREETING", "xin chào, 日本語, 🙂", "APP_GREETING", "none",
          "xin chào, 日本語, 🙂");
    check("case", "APP_PORT", "8080", "app_port", "80", "80");
    check("inherited-name", NULL, NULL, "toString", "none", "none");
    // A C string holds bytes, so a value that is not UTF-8 comes back unchanged.
    check("not-utf8", "APP_NAME", "caf\xe9", "APP_NAME", "none", "caf\xe9");
    if (failures != 0) {
        return 1;
    }
    puts("all cases pass");
    return 0;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

#include "check_setting.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *where, const char *text, const char *want) {
    char got[300];
    check_setting(where, text, got, sizeof got);
    if (strcmp(got, want) != 0) {
        fprintf(stderr, "case %s failed: got %s\n", name, got);
        failures++;
    }
}

int main(void) {
    check("valid", "app.conf:3", "8080", "port 8080");
    check("not-a-number", "app.conf:3", "x", "error: app.conf:3: read port: not a number: 'x'");
    check("too-large", "app.conf:7", "70000", "error: app.conf:7: read port: out of range: 70000");
    check("empty", "app.conf:1", "", "error: app.conf:1: read port: not a number: ''");
    check("zero", "app.conf:2", "0", "error: app.conf:2: read port: out of range: 0");
    check("largest", "app.conf:2", "65535", "port 65535");
    check("leading-space", "app.conf:4", " 80", "error: app.conf:4: read port: not a number: ' 80'");
    check("overflow", "app.conf:5", "99999999999999999999", "error: app.conf:5: read port: out of range: 99999999999999999999");
    check("negative", "app.conf:6", "-1", "error: app.conf:6: read port: not a number: '-1'");

    int port;
    struct setting_error err;
    if (read_port("app.conf:3", "x", &port, &err) != SETTING_NOT_A_NUMBER || err.code != SETTING_NOT_A_NUMBER) {
        fprintf(stderr, "case cause failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}

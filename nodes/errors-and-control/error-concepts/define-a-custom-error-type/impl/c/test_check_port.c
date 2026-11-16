// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

#include "check_port.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *text, const char *want) {
    char got[128];
    check_port(text, got, sizeof got);
    if (strcmp(got, want) != 0) {
        fprintf(stderr, "case %s failed: got %s\n", name, got);
        failures++;
    }
}

int main(void) {
    check("valid", "8080", "port 8080");
    check("not-a-number", "x80", "invalid port 'x80': not a number");
    check("too-large", "65536", "invalid port '65536': out of range");
    check("empty", "", "invalid port '': empty");
    check("zero", "0", "invalid port '0': out of range");
    check("one", "1", "port 1");
    check("largest", "65535", "port 65535");
    check("leading-zeros", "0080", "port 80");
    check("negative", "-1", "invalid port '-1': not a number");
    check("leading-space", " 80", "invalid port ' 80': not a number");
    check("overflow", "99999999999999999999", "invalid port '99999999999999999999': out of range");

    int port;
    struct port_error err;
    if (parse_port("x80", &port, &err) != -1 || strcmp(err.text, "x80") != 0 || err.reason != PORT_NOT_A_NUMBER) {
        fprintf(stderr, "case fields failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}

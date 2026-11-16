// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

#include "check_port.h"

#include <stdio.h>

const char *port_reason_name(enum port_reason reason) {
    switch (reason) {
    case PORT_EMPTY:
        return "empty";
    case PORT_NOT_A_NUMBER:
        return "not a number";
    case PORT_OUT_OF_RANGE:
        return "out of range";
    }
    return "unknown";
}

static int fail(struct port_error *err, const char *text, enum port_reason reason) {
    err->text = text;
    err->reason = reason;
    return -1;
}

int parse_port(const char *text, int *port, struct port_error *err) {
    if (text[0] == '\0') {
        return fail(err, text, PORT_EMPTY);
    }
    for (const char *p = text; *p != '\0'; p++) {
        if (*p < '0' || *p > '9') {
            return fail(err, text, PORT_NOT_A_NUMBER);
        }
    }
    long value = 0;
    for (const char *p = text; *p != '\0'; p++) {
        value = value * 10 + (*p - '0');
        if (value > 65535) {
            return fail(err, text, PORT_OUT_OF_RANGE);
        }
    }
    if (value == 0) {
        return fail(err, text, PORT_OUT_OF_RANGE);
    }
    *port = (int)value;
    return 0;
}

int check_port(const char *text, char *out, size_t size) {
    int port;
    struct port_error err;
    if (parse_port(text, &port, &err) != 0) {
        return snprintf(out, size, "invalid port '%s': %s", err.text, port_reason_name(err.reason));
    }
    return snprintf(out, size, "port %d", port);
}

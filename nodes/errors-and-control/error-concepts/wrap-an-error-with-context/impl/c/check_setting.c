// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

#include "check_setting.h"

#include <stdio.h>
#include <string.h>

int parse_port(const char *text, int *port, struct setting_error *err) {
    int digits = text[0] != '\0';
    for (const char *p = text; *p != '\0'; p++) {
        if (*p < '0' || *p > '9') {
            digits = 0;
        }
    }
    if (!digits) {
        err->code = SETTING_NOT_A_NUMBER;
        snprintf(err->message, sizeof err->message, "not a number: '%s'", text);
        return err->code;
    }
    long value = 0;
    for (const char *p = text; *p != '\0'; p++) {
        value = value * 10 + (*p - '0');
        if (value > 65535) {
            break;
        }
    }
    if (value == 0 || value > 65535) {
        err->code = SETTING_OUT_OF_RANGE;
        snprintf(err->message, sizeof err->message, "out of range: %s", text);
        return err->code;
    }
    *port = (int)value;
    return 0;
}

int read_port(const char *where, const char *text, int *port, struct setting_error *err) {
    int code = parse_port(text, port, err);
    if (code != 0) {
        char cause[sizeof err->message];
        memcpy(cause, err->message, sizeof cause);
        int n = snprintf(err->message, sizeof err->message, "%s: read port: %s", where, cause);
        if (n >= (int)sizeof err->message) {
            // The message was cut to fit. End it with "..." to show the cut.
            memcpy(err->message + sizeof err->message - 4, "...", 4);
        }
    }
    return code;
}

int check_setting(const char *where, const char *text, char *out, size_t size) {
    int port;
    struct setting_error err;
    if (read_port(where, text, &port, &err) != 0) {
        return snprintf(out, size, "error: %s", err.message);
    }
    return snprintf(out, size, "port %d", port);
}

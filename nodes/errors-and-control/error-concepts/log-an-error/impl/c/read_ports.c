// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

#include "read_ports.h"

int parse_port(const char *text, int *port, char *message, size_t size) {
    int digits = text[0] != '\0';
    for (const char *p = text; *p != '\0'; p++) {
        if (*p < '0' || *p > '9') {
            digits = 0;
        }
    }
    if (!digits) {
        snprintf(message, size, "not a number: '%s'", text);
        return 1;
    }
    long value = 0;
    for (const char *p = text; *p != '\0'; p++) {
        value = value * 10 + (*p - '0');
        if (value > 65535) {
            break;
        }
    }
    if (value == 0 || value > 65535) {
        snprintf(message, size, "out of range: %s", text);
        return 2;
    }
    *port = (int)value;
    return 0;
}

void write_quoted(FILE *f, const char *value) {
    fputc('"', f);
    for (const char *p = value; *p != '\0'; p++) {
        switch (*p) {
        case '\\': fputs("\\\\", f); break;
        case '"': fputs("\\\"", f); break;
        case '\n': fputs("\\n", f); break;
        case '\r': fputs("\\r", f); break;
        case '\t': fputs("\\t", f); break;
        default: fputc(*p, f);
        }
    }
    fputc('"', f);
}

size_t read_ports(const char *const texts[], size_t n, int ports[], FILE *log) {
    size_t ok = 0;
    char message[256];
    for (size_t i = 0; i < n; i++) {
        int port;
        if (parse_port(texts[i], &port, message, sizeof message) != 0) {
            // The error is handled here: it is logged once and not returned.
            fprintf(log, "level=ERROR msg=\"read port failed\" line=%zu err=", i + 1);
            write_quoted(log, message);
            fputc('\n', log);
            continue;
        }
        ports[ok++] = port;
    }
    fprintf(log, "level=INFO msg=\"ports read\" ok=%zu total=%zu\n", ok, n);
    return ok;
}

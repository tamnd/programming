// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

#include "wrap.h"

#include <ctype.h>
#include <string.h>

int wrap(const char *text, size_t width, char *out, size_t size) {
    size_t used = 0;
    size_t line_len = 0;
    int lines = 0;
    const char *p = text;
    for (;;) {
        while (*p != '\0' && isspace((unsigned char)*p)) {
            p++;
        }
        if (*p == '\0') {
            break;
        }
        const char *start = p;
        while (*p != '\0' && !isspace((unsigned char)*p)) {
            p++;
        }
        size_t len = (size_t)(p - start);
        if (line_len > 0 && line_len + 1 + len <= width) {
            if (used + 1 + len + 2 > size) {
                return -1;
            }
            out[used++] = ' ';
            line_len++;
        } else {
            if (line_len > 0) {
                out[used++] = '\n';
            }
            if (used + len + 2 > size) {
                return -1;
            }
            line_len = 0;
            lines++;
        }
        memcpy(out + used, start, len);
        used += len;
        line_len += len;
    }
    if (lines > 0) {
        out[used++] = '\n';
    }
    out[used] = '\0';
    return lines;
}

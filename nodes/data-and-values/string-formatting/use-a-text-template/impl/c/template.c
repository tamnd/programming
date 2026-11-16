// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

#include "template.h"

#include <ctype.h>
#include <string.h>

// put copies n bytes of s to out at *len, as far as size allows, and adds n to *len.
static void put(char *out, size_t size, size_t *len, const char *s, size_t n) {
    for (size_t i = 0; i < n; i++, (*len)++) {
        if (*len + 1 < size) {
            out[*len] = s[i];
        }
    }
}

static size_t name_length(const char *s) {
    size_t n = 0;
    if (isalpha((unsigned char)s[0]) || s[0] == '_') {
        n = 1;
        while (isalnum((unsigned char)s[n]) || s[n] == '_') {
            n++;
        }
    }
    return n;
}

int substitute(char *out, size_t size, const char *template, const char *const names[],
               const char *const values[], size_t count) {
    size_t len = 0;
    const char *p = template;
    for (;;) {
        const char *dollar = strchr(p, '$');
        if (dollar == NULL) {
            put(out, size, &len, p, strlen(p));
            break;
        }
        put(out, size, &len, p, (size_t)(dollar - p));
        p = dollar + 1;
        if (*p == '$') {
            put(out, size, &len, "$", 1);
            p++;
            continue;
        }
        int braced = *p == '{';
        const char *name = braced ? p + 1 : p;
        size_t n = name_length(name);
        if (n == 0 || (braced && name[n] != '}')) {
            return -1;
        }
        size_t i = 0;
        while (i < count && (strlen(names[i]) != n || strncmp(names[i], name, n) != 0)) {
            i++;
        }
        if (i == count) {
            return -1;
        }
        put(out, size, &len, values[i], strlen(values[i]));
        p = name + n + (braced ? 1 : 0);
    }
    if (size > 0) {
        out[len < size ? len : size - 1] = '\0';
    }
    return (int)len;
}

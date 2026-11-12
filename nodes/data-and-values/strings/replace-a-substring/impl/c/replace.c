// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

#include "replace.h"

#include <stdlib.h>
#include <string.h>

// replace_n replaces at most limit matches. The first pass counts the
// matches, so the second pass can write into one allocation of the right size.
static char *replace_n(const char *s, const char *old, const char *new_, size_t limit) {
    size_t old_len = strlen(old);
    size_t new_len = strlen(new_);
    size_t count = 0;
    for (const char *p = strstr(s, old); p != NULL && count < limit; p = strstr(p + old_len, old)) {
        count++;
    }
    size_t len = strlen(s) - count * old_len + count * new_len;
    char *out = malloc(len + 1);
    if (out == NULL) {
        return NULL;
    }
    char *w = out;
    const char *p = s;
    for (size_t i = 0; i < count; i++) {
        const char *m = strstr(p, old);
        memcpy(w, p, (size_t)(m - p));
        w += m - p;
        memcpy(w, new_, new_len);
        w += new_len;
        p = m + old_len;
    }
    strcpy(w, p);
    return out;
}

char *replace(const char *s, const char *old, const char *new_) {
    return replace_n(s, old, new_, (size_t)-1);
}

char *replace_first(const char *s, const char *old, const char *new_) {
    return replace_n(s, old, new_, 1);
}

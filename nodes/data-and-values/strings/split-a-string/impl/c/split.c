// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

#define _POSIX_C_SOURCE 200809L

#include "split.h"

#include <regex.h>
#include <stdlib.h>
#include <string.h>

// push appends a field to the array *fields of *count elements, which has
// room for *cap. It returns 0, or -1 if there is no memory.
static int push(struct field **fields, size_t *count, size_t *cap, const char *start, size_t len) {
    if (*count == *cap) {
        size_t n = *cap == 0 ? 8 : *cap * 2;
        struct field *p = realloc(*fields, n * sizeof **fields);
        if (p == NULL) {
            return -1;
        }
        *fields = p;
        *cap = n;
    }
    (*fields)[(*count)++] = (struct field){start, len};
    return 0;
}

struct field *split(const char *s, const char *sep, size_t limit, size_t *count) {
    struct field *fields = NULL;
    size_t n = 0, cap = 0, seplen = strlen(sep);
    const char *at;
    // strstr finds the next occurrence. The search goes on after its end, so
    // the occurrences do not overlap.
    while ((limit == 0 || n + 1 < limit) && (at = strstr(s, sep)) != NULL) {
        if (push(&fields, &n, &cap, s, (size_t)(at - s)) != 0) {
            free(fields);
            return NULL;
        }
        s = at + seplen;
    }
    if (push(&fields, &n, &cap, s, strlen(s)) != 0) {
        free(fields);
        return NULL;
    }
    *count = n;
    return fields;
}

struct field *split_whitespace(const char *s, size_t *count) {
    static const char space[] = " \t\n";
    struct field *fields = malloc(sizeof *fields);
    size_t n = 0, cap = 1;
    if (fields == NULL) {
        return NULL;
    }
    for (;;) {
        s += strspn(s, space);
        if (*s == '\0') {
            break;
        }
        size_t len = strcspn(s, space);
        if (push(&fields, &n, &cap, s, len) != 0) {
            free(fields);
            return NULL;
        }
        s += len;
    }
    *count = n;
    return fields;
}

struct field *split_regex(const char *s, const char *pattern, size_t *count) {
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
        return NULL;
    }
    struct field *fields = NULL;
    size_t n = 0, cap = 0;
    regmatch_t m;
    int flags = 0;
    // After the first match, the rest of s does not start at a line start, so
    // REG_NOTBOL stops a ^ in the pattern from matching there.
    while (regexec(&re, s, 1, &m, flags) == 0 && m.rm_eo > m.rm_so) {
        if (push(&fields, &n, &cap, s, (size_t)m.rm_so) != 0) {
            goto fail;
        }
        s += m.rm_eo;
        flags = REG_NOTBOL;
    }
    if (push(&fields, &n, &cap, s, strlen(s)) != 0) {
        goto fail;
    }
    regfree(&re);
    *count = n;
    return fields;
fail:
    regfree(&re);
    free(fields);
    return NULL;
}

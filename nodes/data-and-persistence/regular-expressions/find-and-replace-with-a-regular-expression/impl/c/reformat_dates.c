// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

// POSIX regex.h compiles and matches a pattern, but it has no function to replace a match and no function to escape
// a literal string. This file has both.

#define _POSIX_C_SOURCE 200809L

#include "reformat_dates.h"

#include <regex.h>
#include <stdlib.h>
#include <string.h>

// A growing string.
struct buf {
    char *p;
    size_t n, cap;
};

static bool append(struct buf *b, const char *s, size_t len) {
    if (b->n + len + 1 > b->cap) {
        size_t cap = b->cap ? b->cap : 64;
        while (b->n + len + 1 > cap) {
            cap *= 2;
        }
        char *p = realloc(b->p, cap);
        if (p == nullptr) {
            return false;
        }
        b->p = p;
        b->cap = cap;
    }
    memcpy(b->p + b->n, s, len);
    b->n += len;
    b->p[b->n] = '\0';
    return true;
}

// Appends s to b with a backslash before each special character of an extended regular expression.
static bool append_escaped(struct buf *b, const char *s) {
    for (; *s != '\0'; s++) {
        if (strchr(".[\\()*+?{|^$", *s) != nullptr && !append(b, "\\", 1)) {
            return false;
        }
        if (!append(b, s, 1)) {
            return false;
        }
    }
    return true;
}

// Compiles the pattern of a date YYYY<sep>MM<sep>DD. ERE has no lookaround, so the pattern does not check the
// digits next to the date.
static bool date_pattern(regex_t *re, const char *sep) {
    struct buf b = {0};
    bool ok = append(&b, "([0-9]{4})", 10) && append_escaped(&b, sep) && append(&b, "([0-9]{2})", 10) &&
              append_escaped(&b, sep) && append(&b, "([0-9]{2})", 10);
    ok = ok && regcomp(re, b.p, REG_EXTENDED) == 0;
    free(b.p);
    return ok;
}

static bool is_digit_at(const char *text, size_t len, size_t i) {
    return i < len && text[i] >= '0' && text[i] <= '9';
}

// Finds the next date at or after *pos that has no digit next to it. On success, m holds offsets in text.
static bool next_date(const regex_t *re, const char *text, size_t len, size_t *pos, regmatch_t m[4]) {
    while (*pos < len) {
        if (regexec(re, text + *pos, 4, m, 0) != 0) {
            return false;
        }
        for (int i = 0; i < 4; i++) {
            m[i].rm_so += (regoff_t)*pos;
            m[i].rm_eo += (regoff_t)*pos;
        }
        size_t start = (size_t)m[0].rm_so;
        size_t end = (size_t)m[0].rm_eo;
        if ((start > 0 && is_digit_at(text, len, start - 1)) || is_digit_at(text, len, end)) {
            *pos = start + 1; // a date can start inside this match
            continue;
        }
        *pos = end;
        return true;
    }
    return false;
}

static bool append_group(struct buf *b, const char *text, regmatch_t g) {
    return append(b, text + g.rm_so, (size_t)(g.rm_eo - g.rm_so));
}

char *reformat_dates(const char *text, const char *from_sep, const char *to_sep) {
    regex_t re;
    if (!date_pattern(&re, from_sep)) {
        return nullptr;
    }
    size_t len = strlen(text);
    size_t sep_len = strlen(to_sep);
    struct buf out = {0};
    bool ok = append(&out, "", 0);
    size_t pos = 0;
    size_t last = 0;
    regmatch_t m[4];
    while (ok && next_date(&re, text, len, &pos, m)) {
        ok = append(&out, text + last, (size_t)m[0].rm_so - last) && append_group(&out, text, m[3]) &&
             append(&out, to_sep, sep_len) && append_group(&out, text, m[2]) && append(&out, to_sep, sep_len) &&
             append_group(&out, text, m[1]);
        last = (size_t)m[0].rm_eo;
    }
    ok = ok && append(&out, text + last, len - last);
    regfree(&re);
    if (!ok) {
        free(out.p);
        return nullptr;
    }
    return out.p;
}

int find_dates(const char *text, const char *sep, struct date **out, size_t *n) {
    regex_t re;
    if (!date_pattern(&re, sep)) {
        return -1;
    }
    size_t len = strlen(text);
    struct date *dates = nullptr;
    size_t count = 0;
    size_t pos = 0;
    regmatch_t m[4];
    while (next_date(&re, text, len, &pos, m)) {
        struct date *p = realloc(dates, (count + 1) * sizeof *p);
        if (p == nullptr) {
            free(dates);
            regfree(&re);
            return -1;
        }
        dates = p;
        struct date *d = &dates[count++];
        memcpy(d->year, text + m[1].rm_so, 4);
        d->year[4] = '\0';
        memcpy(d->month, text + m[2].rm_so, 2);
        d->month[2] = '\0';
        memcpy(d->day, text + m[3].rm_so, 2);
        d->day[2] = '\0';
    }
    regfree(&re);
    *out = dates;
    *n = count;
    return 0;
}

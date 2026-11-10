// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

#include "unique.h"

#include <string.h>

// This version uses no hash table. It compares each item with the items that
// it kept, so the time grows with n * n.
size_t unique(const char **items, size_t n) {
    size_t kept = 0;
    for (size_t i = 0; i < n; i++) {
        size_t j = 0;
        while (j < kept && strcmp(items[j], items[i]) != 0) {
            j++;
        }
        if (j == kept) {
            items[kept++] = items[i];
        }
    }
    return kept;
}

// Not tolower or strcasecmp: they use the current locale.
static unsigned char ascii_lower(unsigned char c) {
    return c >= 'A' && c <= 'Z' ? (unsigned char)(c - 'A' + 'a') : c;
}

static bool same_email(const char *a, const char *b) {
    for (;; a++, b++) {
        unsigned char x = ascii_lower((unsigned char)*a);
        if (x != ascii_lower((unsigned char)*b)) {
            return false;
        }
        if (x == '\0') {
            return true;
        }
    }
}

size_t unique_by_email(struct user *users, size_t n) {
    size_t kept = 0;
    for (size_t i = 0; i < n; i++) {
        size_t j = 0;
        while (j < kept && !same_email(users[j].email, users[i].email)) {
            j++;
        }
        if (j == kept) {
            users[kept++] = users[i];
        }
    }
    return kept;
}

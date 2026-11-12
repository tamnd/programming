// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

#ifndef UNIQUE_H
#define UNIQUE_H

#include <stddef.h>

struct user {
    const char *name;
    const char *email;  // ASCII
};

// unique removes the later copies of equal strings from items[0..n) in place.
// The first copies keep their order. It returns the new number of items.
// Two strings are equal if they have the same bytes.
size_t unique(const char **items, size_t n);

// unique_by_email keeps the first user for each email in users[0..n), in
// place, and returns the new number of users. Case does not matter in the
// emails.
size_t unique_by_email(struct user *users, size_t n);

#endif

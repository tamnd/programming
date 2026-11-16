// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

#include "birthday.h"

#include <string.h>

void birthday(const struct person *people, size_t n, const char *name, struct person *out) {
    for (size_t i = 0; i < n; i++) {
        out[i] = people[i]; // struct assignment copies every member
        if (strcmp(out[i].name, name) == 0) {
            out[i].age++;
        }
    }
}

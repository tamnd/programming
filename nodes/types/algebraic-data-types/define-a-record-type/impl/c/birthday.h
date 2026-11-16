// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

#ifndef BIRTHDAY_H
#define BIRTHDAY_H

#include <stddef.h>

struct person {
    const char *name; // not owned; the caller keeps the text alive
    int age;
};

// birthday copies the n people to out. Each person whose name is equal to
// name, byte for byte, is one year older in out. out must have room for n
// people. The input does not change.
void birthday(const struct person *people, size_t n, const char *name, struct person *out);

#endif

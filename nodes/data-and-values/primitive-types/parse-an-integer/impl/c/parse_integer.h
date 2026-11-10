// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

#ifndef PARSE_INTEGER_H
#define PARSE_INTEGER_H

#include <stdint.h>

enum parse_status {
    PARSE_OK,      // *out holds the value
    PARSE_INVALID, // s is not an optional sign and ASCII digits of the base
    PARSE_RANGE,   // the value does not fit in int64_t
};

// parse_integer parses the string s as a signed 64-bit integer in the given
// base, from 2 to 36. It writes *out only when it returns PARSE_OK.
enum parse_status parse_integer(const char *s, int base, int64_t *out);

#endif

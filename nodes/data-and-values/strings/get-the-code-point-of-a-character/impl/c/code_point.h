// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.
#ifndef CODE_POINT_H
#define CODE_POINT_H

#include <stdint.h>

// code_point returns the code point of the UTF-8 sequence at the start of c.
uint32_t code_point(const char *c);

#endif

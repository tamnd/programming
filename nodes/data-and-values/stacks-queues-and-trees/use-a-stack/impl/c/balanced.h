// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

#ifndef BALANCED_H
#define BALANCED_H

#include <stdbool.h>

// balanced returns true if the brackets of the string text match and nest
// correctly. It returns false also if it cannot get memory for the stack.
bool balanced(const char *text);

#endif

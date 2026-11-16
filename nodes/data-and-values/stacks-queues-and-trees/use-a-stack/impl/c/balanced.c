// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

#include "balanced.h"

#include <stdlib.h>
#include <string.h>

static char opening(char close) {
    switch (close) {
    case ')':
        return '(';
    case ']':
        return '[';
    default:
        return '{';
    }
}

bool balanced(const char *text) {
    // C has no stack type. An array and a count make one: the count is the
    // index of the next free slot, and the top is at count - 1. The stack
    // never holds more characters than the text has.
    size_t n = strlen(text);
    char *stack = malloc(n + 1);
    if (stack == NULL) {
        return false;
    }
    size_t count = 0;
    bool ok = true;
    for (size_t i = 0; i < n && ok; i++) {
        char c = text[i];
        if (c == '(' || c == '[' || c == '{') {
            stack[count++] = c; // push
        } else if (c == ')' || c == ']' || c == '}') {
            if (count == 0 || stack[count - 1] != opening(c)) {
                ok = false; // check the count before you read the top
            } else {
                count--; // pop
            }
        }
    }
    free(stack);
    return ok && count == 0;
}

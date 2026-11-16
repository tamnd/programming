// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

#include "caesar.h"

void caesar(char *text, long long shift) {
    // In C, % keeps the sign of the left side, so add 26 and take % again.
    int k = (int)(((shift % 26) + 26) % 26);
    for (char *p = text; *p != '\0'; p++) {
        if (*p >= 'A' && *p <= 'Z') {
            *p = (char)('A' + (*p - 'A' + k) % 26);
        } else if (*p >= 'a' && *p <= 'z') {
            *p = (char)('a' + (*p - 'a' + k) % 26);
        }
    }
}

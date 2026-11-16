// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

#include "palindrome.h"

#include <ctype.h>
#include <string.h>

bool is_palindrome(const char *text) {
    size_t n = strlen(text);
    if (n < 2) {
        return true;
    }
    size_t i = 0;
    size_t j = n - 1;
    while (i < j) {
        unsigned char a = (unsigned char)text[i];
        unsigned char b = (unsigned char)text[j];
        if (!isalnum(a)) {
            i++;
        } else if (!isalnum(b)) {
            j--;
        } else if (tolower(a) != tolower(b)) {
            return false;
        } else {
            i++;
            j--;
        }
    }
    return true;
}

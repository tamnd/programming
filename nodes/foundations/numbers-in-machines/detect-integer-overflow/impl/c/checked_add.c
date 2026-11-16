// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.

#include "checked_add.h"

bool checked_add(int64_t a, int64_t b, int64_t *out) {
#if defined(__GNUC__) || defined(__clang__)
    // The builtin adds in infinite precision, stores the sum wrapped to 64
    // bits and returns true when the stored value is not the exact sum.
    return !__builtin_add_overflow(a, b, out);
#else
    return checked_add_portable(a, b, out);
#endif
}

bool checked_add_portable(int64_t a, int64_t b, int64_t *out) {
    // A signed overflow is undefined behavior, so do not compute a sum that
    // overflows. Test the operands first: with b > 0 the sum overflows when
    // a > INT64_MAX - b, and with b < 0 when a < INT64_MIN - b. Neither of
    // these subtractions can overflow.
    if ((b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b)) {
        return false;
    }
    *out = a + b;
    return true;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

#ifndef DIVMOD_HPP
#define DIVMOD_HPP

// The two results of div_mod.
struct div_mod_result {
    int quotient;
    int remainder;
};

// Returns the quotient and the remainder of the floor division of a by b.
inline div_mod_result div_mod(int a, int b) {
    int q = a / b;
    int r = a % b;
    if (r != 0 && (r < 0) != (b < 0)) {
        q -= 1;
        r += b;
    }
    return {q, r};
}

#endif

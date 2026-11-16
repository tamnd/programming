// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

#include "repeat.h"

struct repeated repeat(step_fn f, int times) {
    struct repeated r = {f, times};
    return r;
}

int call_repeated(struct repeated r, int x) {
    for (int i = 0; i < r.times; i++) {
        x = r.f(x);
    }
    return x;
}

int collatz_step(int n) {
    return n % 2 == 0 ? n / 2 : 3 * n + 1;
}

int collatz_after(int start, int times) {
    return call_repeated(repeat(collatz_step, times), start);
}

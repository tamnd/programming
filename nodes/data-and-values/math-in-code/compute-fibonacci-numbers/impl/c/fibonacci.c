// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

#include "fibonacci.h"

long long fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    long long a = 0;
    long long b = 1;
    for (int i = 1; i < n; i++) {
        long long next = a + b;
        a = b;
        b = next;
    }
    return b;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

#include "factorial.h"

long long factorial(int n) {
    long long f = 1;
    for (int i = 2; i <= n; i++) {
        f *= i;
    }
    return f;
}

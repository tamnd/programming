// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

#include "matrix.h"

void transpose(const long long *a, size_t n, size_t m, long long *out) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            out[j * n + i] = a[i * m + j];
        }
    }
}

// The order i, k, j reads b and out row by row, which suits row-major order.
void matrix_multiply(const long long *a, const long long *b, size_t n, size_t m, size_t p, long long *out) {
    for (size_t i = 0; i < n * p; i++) {
        out[i] = 0;
    }
    for (size_t i = 0; i < n; i++) {
        for (size_t k = 0; k < m; k++) {
            long long x = a[i * m + k];
            for (size_t j = 0; j < p; j++) {
                out[i * p + j] += x * b[k * p + j];
            }
        }
    }
}

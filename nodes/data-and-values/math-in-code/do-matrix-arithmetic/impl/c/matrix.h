// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

#ifndef MATRIX_ARITHMETIC_H
#define MATRIX_ARITHMETIC_H

#include <stddef.h>

// The matrices are flat arrays in row-major order: the item in row i and
// column j of an n x m matrix is at index i * m + j.

// Sets out (m x n) to the transpose of a (n x m).
void transpose(const long long *a, size_t n, size_t m, long long *out);

// Sets out (n x p) to the product of a (n x m) and b (m x p).
void matrix_multiply(const long long *a, const long long *b, size_t n, size_t m, size_t p, long long *out);

#endif

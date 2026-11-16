// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

#ifndef MATRIX_ARITHMETIC_HPP
#define MATRIX_ARITHMETIC_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

using Matrix = std::vector<std::vector<long long>>;

// Returns the transpose of m: row i of the result is column i of m.
inline Matrix transpose(const Matrix& m) {
    Matrix t(m[0].size(), std::vector<long long>(m.size()));
    for (std::size_t i = 0; i < m.size(); ++i) {
        for (std::size_t j = 0; j < m[0].size(); ++j) {
            t[j][i] = m[i][j];
        }
    }
    return t;
}

// Returns the product a times b. Throws std::invalid_argument if the number
// of columns of a is not the number of rows of b.
inline Matrix matrix_multiply(const Matrix& a, const Matrix& b) {
    if (a[0].size() != b.size()) {
        throw std::invalid_argument("the sizes do not match");
    }
    Matrix c(a.size(), std::vector<long long>(b[0].size()));
    for (std::size_t i = 0; i < a.size(); ++i) {
        for (std::size_t k = 0; k < b.size(); ++k) {
            for (std::size_t j = 0; j < b[0].size(); ++j) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

#endif

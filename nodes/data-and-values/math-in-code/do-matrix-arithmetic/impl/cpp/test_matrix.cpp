// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

#include <cstdio>
#include <stdexcept>

#include "matrix.hpp"

namespace {

struct test_case {
    const char* name;
    Matrix a, b, want;
};

const test_case cases[] = {
    {"two-by-two", {{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}, {{19, 22}, {43, 50}}},
    {"two-by-three-times-three-by-two", {{1, 2, 3}, {4, 5, 6}}, {{7, 8}, {9, 10}, {11, 12}}, {{58, 64}, {139, 154}}},
    {"one-by-one", {{3}}, {{4}}, {{12}}},
    {"row-times-column", {{1, 2, 3}}, {{4}, {5}, {6}}, {{32}}},
    {"column-times-row", {{1}, {2}, {3}}, {{4, 5, 6}}, {{4, 5, 6}, {8, 10, 12}, {12, 15, 18}}},
    {"identity", {{1, 0}, {0, 1}}, {{5, 6}, {7, 8}}, {{5, 6}, {7, 8}}},
    {"other-order", {{5, 6}, {7, 8}}, {{1, 2}, {3, 4}}, {{23, 34}, {31, 46}}},
    {"zero-matrix", {{0, 0}, {0, 0}}, {{1, 2}, {3, 4}}, {{0, 0}, {0, 0}}},
    {"negative-numbers", {{-1, 2}, {3, -4}}, {{2, 0}, {1, -1}}, {{0, -2}, {2, 4}}},
    {"beyond-32-bit", {{100000, 0}, {0, 100000}}, {{100000}, {3}}, {{10000000000LL}, {300000}}},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (matrix_multiply(c.a, c.b) != c.want) {
            std::printf("FAIL %s\n", c.name);
            ++failures;
        }
    }
    if (transpose({{1, 2, 3}, {4, 5, 6}}) != Matrix{{1, 4}, {2, 5}, {3, 6}}) {
        std::printf("FAIL transpose\n");
        ++failures;
    }
    try {
        matrix_multiply({{1, 2}}, {{1, 2}});
        std::printf("FAIL no exception\n");
        ++failures;
    } catch (const std::invalid_argument&) {
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}

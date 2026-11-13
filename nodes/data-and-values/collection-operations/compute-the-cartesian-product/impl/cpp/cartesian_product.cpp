// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

#include "cartesian_product.hpp"

std::vector<std::vector<std::int32_t>> cartesian_product(const std::vector<std::vector<std::int32_t>> &lists) {
    // The product of no lists is one empty tuple.
    std::vector<std::vector<std::int32_t>> tuples{{}};
    for (const auto &xs : lists) {
        std::vector<std::vector<std::int32_t>> next;
        next.reserve(tuples.size() * xs.size());
        for (const auto &t : tuples) {
            for (std::int32_t x : xs) {
                auto u = t;
                u.push_back(x);
                next.push_back(std::move(u));
            }
        }
        tuples = std::move(next);
    }
    return tuples;
}

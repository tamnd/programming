// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

#ifndef SUM_AND_PRODUCT_HPP
#define SUM_AND_PRODUCT_HPP

#include <functional>
#include <numeric>
#include <utility>
#include <vector>

// Returns the sum and the product of the items. The start values 0LL and
// 1LL set the type of the accumulator.
inline std::pair<long long, long long> sum_and_product(const std::vector<long long>& items) {
    return {std::accumulate(items.begin(), items.end(), 0LL),
            std::accumulate(items.begin(), items.end(), 1LL, std::multiplies<>())};
}

#endif

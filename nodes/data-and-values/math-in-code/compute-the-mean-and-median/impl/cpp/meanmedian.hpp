// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

#ifndef MEAN_AND_MEDIAN_HPP
#define MEAN_AND_MEDIAN_HPP

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

// Returns the mean and the median of a non-empty vector. The start value
// 0.0 makes the sum a double. nth_element works on a copy.
inline std::pair<double, double> mean_and_median(const std::vector<long long>& items) {
    if (items.empty()) {
        throw std::invalid_argument("the list is empty");
    }
    const std::size_t n = items.size();
    const double mean =
        std::accumulate(items.begin(), items.end(), 0.0,
                        [](double a, long long x) { return a + static_cast<double>(x); }) /
        static_cast<double>(n);
    std::vector<long long> s = items;
    const auto mid = s.begin() + static_cast<std::ptrdiff_t>(n / 2);
    std::nth_element(s.begin(), mid, s.end());
    double median = static_cast<double>(*mid);
    if (n % 2 == 0) {
        // The elements before mid are not larger, so the lower middle is their maximum.
        const double low = static_cast<double>(*std::max_element(s.begin(), mid));
        median = (low + median) / 2;
    }
    return {mean, median};
}

#endif

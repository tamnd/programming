// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

#ifndef GCD_AND_LCM_HPP
#define GCD_AND_LCM_HPP

#include <numeric>
#include <utility>

// Returns the greatest common divisor and the least common multiple of a and
// b. std::gcd and std::lcm have undefined behavior when a result does not
// fit, so neither a nor b can be LLONG_MIN.
inline std::pair<long long, long long> gcd_and_lcm(long long a, long long b) {
    return {std::gcd(a, b), std::lcm(a, b)};
}

#endif

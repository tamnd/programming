// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.

#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

// Returns base to the power exp as decimal text. -1000 <= base <= 1000.
// The digits live in base-10^9 limbs, least significant first.
inline std::string big_power(int base, int exp) {
    constexpr std::uint64_t limb = 1000000000;
    const std::uint64_t m = base < 0 ? -static_cast<std::int64_t>(base) : base;
    std::vector<std::uint32_t> limbs{1};
    for (int i = 0; i < exp; i++) {
        std::uint64_t carry = 0;
        for (auto &d : limbs) {
            const std::uint64_t t = d * m + carry;
            d = static_cast<std::uint32_t>(t % limb);
            carry = t / limb;
        }
        if (carry != 0) {
            limbs.push_back(static_cast<std::uint32_t>(carry));
        }
    }
    std::string out = (base < 0 && exp % 2 == 1) ? "-" : "";
    out += std::to_string(limbs.back());
    char buf[16];
    for (auto it = limbs.rbegin() + 1; it != limbs.rend(); ++it) {
        std::snprintf(buf, sizeof buf, "%09u", static_cast<unsigned>(*it)); // keep the inner zeros
        out += buf;
    }
    return out;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

#include "vec2.hpp"

#include <stdexcept>

namespace vec2 {

Vec2 vector_sum(const std::vector<std::int64_t>& xs, const std::vector<std::int64_t>& ys) {
    if (xs.size() != ys.size()) {
        throw std::invalid_argument("the lists must have the same length");
    }
    Vec2 total;
    for (std::size_t i = 0; i < xs.size(); i++) {
        total = total + Vec2{xs[i], ys[i]};
    }
    return total;
}

}  // namespace vec2

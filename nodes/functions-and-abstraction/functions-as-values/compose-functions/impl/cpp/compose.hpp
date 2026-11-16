// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

#ifndef COMPOSE_HPP
#define COMPOSE_HPP

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace composition {

// Returns the composition of the functions. The last one runs first.
template <typename T>
std::function<T(T)> compose(std::vector<std::function<T(T)>> fs) {
    return [fs = std::move(fs)](T x) {
        for (auto it = fs.rbegin(); it != fs.rend(); ++it) {
            x = (*it)(std::move(x));
        }
        return x;
    };
}

// Composes the functions that ops names and applies the result to x.
int compose_all(const std::vector<std::string>& ops, int x);

}  // namespace composition

#endif

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

#include "compose.hpp"

#include <map>

namespace composition {

int compose_all(const std::vector<std::string>& ops, int x) {
    static const std::map<std::string, std::function<int(int)>> table = {
        {"inc", [](int v) { return v + 1; }},
        {"dec", [](int v) { return v - 1; }},
        {"double", [](int v) { return 2 * v; }},
        {"negate", [](int v) { return -v; }},
    };
    std::vector<std::function<int(int)>> fs;
    fs.reserve(ops.size());
    for (const auto& name : ops) {
        fs.push_back(table.at(name));
    }
    return compose(std::move(fs))(x);
}

}  // namespace composition

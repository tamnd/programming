// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

#include "count_points.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_map>

namespace {

// Point is a map key. std::unordered_map needs == and a hash, and equal
// points must have equal hashes.
struct Point {
    int x;
    int y;
    bool operator==(const Point &) const = default;
};

struct PointHash {
    std::size_t operator()(const Point &p) const noexcept {
        // Put the two 32-bit values in one 64-bit value, then hash it.
        std::uint64_t bits = static_cast<std::uint64_t>(static_cast<std::uint32_t>(p.x)) << 32 | static_cast<std::uint32_t>(p.y);
        return std::hash<std::uint64_t>{}(bits);
    }
};

}  // namespace

std::vector<std::tuple<int, int, int>> count_points(const std::vector<std::pair<int, int>> &points) {
    std::unordered_map<Point, int, PointHash> counts;
    for (const auto &[x, y] : points) {
        counts[Point{x, y}]++;
    }
    std::vector<std::tuple<int, int, int>> out;
    out.reserve(counts.size());
    for (const auto &[p, n] : counts) {
        out.emplace_back(p.x, p.y, n);
    }
    std::ranges::sort(out);
    return out;
}

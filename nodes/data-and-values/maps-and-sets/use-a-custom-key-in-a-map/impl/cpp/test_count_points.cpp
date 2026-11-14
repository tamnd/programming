// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

#include "count_points.hpp"

#include <iostream>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

static int failures = 0;

using Points = std::vector<std::pair<int, int>>;
using Triples = std::vector<std::tuple<int, int, int>>;

static void check(std::string_view name, const Points &points, const Triples &want) {
    if (count_points(points) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("clicks", Points{{1, 2}, {3, 4}, {1, 2}}, Triples{{1, 2, 2}, {3, 4, 1}});
    check("swap", Points{{2, 1}, {1, 2}}, Triples{{1, 2, 1}, {2, 1, 1}});
    check("empty", Points{}, Triples{});
    check("one", Points{{0, 0}}, Triples{{0, 0, 1}});
    check("negative", Points{{-1, 0}, {0, -1}, {-1, 0}}, Triples{{-1, 0, 2}, {0, -1, 1}});
    check("joined-digits", Points{{1, 23}, {12, 3}}, Triples{{1, 23, 1}, {12, 3, 1}});
    check("all-same", Points{{5, 5}, {5, 5}, {5, 5}}, Triples{{5, 5, 3}});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

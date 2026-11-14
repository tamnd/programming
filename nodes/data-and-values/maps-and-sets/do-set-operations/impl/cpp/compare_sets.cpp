// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

#include "compare_sets.hpp"

#include <algorithm>
#include <iterator>

// sort_unique makes a sorted vector with no repeats, which is the form that the
// set algorithms need.
static void sort_unique(std::vector<std::string> &v) {
    std::ranges::sort(v);
    auto repeats = std::ranges::unique(v);
    v.erase(repeats.begin(), repeats.end());
}

SetOps compare_sets(std::vector<std::string> a, std::vector<std::string> b) {
    sort_unique(a);
    sort_unique(b);
    SetOps r;
    std::ranges::set_union(a, b, std::back_inserter(r.either));
    std::ranges::set_intersection(a, b, std::back_inserter(r.both));
    std::ranges::set_difference(a, b, std::back_inserter(r.only_a));
    std::ranges::set_symmetric_difference(a, b, std::back_inserter(r.exactly_one));
    // includes(x, y) tells if y is in x, so the larger set comes first.
    r.a_in_b = std::ranges::includes(b, a);
    return r;
}

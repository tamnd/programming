// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

#pragma once

#include <string>
#include <vector>

// SetOps has the results of the set operations. Each vector is in ascending
// order. The names are not union and so on, because union is a keyword.
struct SetOps {
    std::vector<std::string> either;      // the union
    std::vector<std::string> both;        // the intersection
    std::vector<std::string> only_a;      // the difference a minus b
    std::vector<std::string> exactly_one; // the symmetric difference
    bool a_in_b = false;                  // the set of a is a subset of the set of b

    bool operator==(const SetOps &) const = default;
};

// compare_sets does the set operations on the sets of a and b.
SetOps compare_sets(std::vector<std::string> a, std::vector<std::string> b);

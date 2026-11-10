// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

#include "sort_employees.hpp"

#include <algorithm>
#include <tuple>

void sort_employees(std::vector<Employee>& employees) {
    // std::sort is not stable. std::ranges::stable_sort is.
    std::ranges::stable_sort(employees, [](const Employee& a, const Employee& b) {
        // std::tie compares the items in turn. b.salary is on the left, so high comes first.
        // std::string compares chars as unsigned char, so UTF-8 text sorts by code point.
        return std::tie(a.dept, b.salary) < std::tie(b.dept, a.salary);
    });
}

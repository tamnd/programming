// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

#pragma once

#include <string>
#include <vector>

struct Employee {
    std::string name;
    std::string dept;
    long salary;  // long has at least 32 bits
    bool operator==(const Employee&) const = default;
};

// Sorts the records in place by dept, then by salary from high to low.
// Records with equal keys keep their input order.
void sort_employees(std::vector<Employee>& employees);

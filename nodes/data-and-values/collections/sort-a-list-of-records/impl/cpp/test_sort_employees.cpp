// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list-of-records/.

#include "sort_employees.hpp"

#include <cstdio>
#include <format>
#include <string>
#include <vector>

namespace {

using List = std::vector<Employee>;

int failures = 0;

void check(const char* name, List given, const List& want) {
    sort_employees(given);
    if (given != want) {
        std::printf("FAIL %s\n", name);
        ++failures;
    } else {
        std::printf("ok %s\n", name);
    }
}

// The 40 records of the many-ties case: depts "b" and "a" in turn, all with salary 1.
// With parity 0 or 1, only the records at even or odd positions.
List many(int parity) {
    List out;
    for (int i = 0; i < 40; ++i) {
        if (parity >= 0 && i % 2 != parity) continue;
        out.push_back({std::format("n{:02}", i + 1), i % 2 == 1 ? "a" : "b", 1});
    }
    return out;
}

}  // namespace

int main() {
    check("mixed", {{"Ada", "eng", 120}, {"Bob", "sales", 90}, {"Cy", "eng", 150}, {"Di", "ops", 100}},
          {{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Di", "ops", 100}, {"Bob", "sales", 90}});
    check("same-salary-other-dept", {{"Ann", "ops", 100}, {"Ben", "eng", 100}, {"Cat", "hr", 100}},
          {{"Ben", "eng", 100}, {"Cat", "hr", 100}, {"Ann", "ops", 100}});
    check("empty", {}, {});
    check("single", {{"Ada", "eng", 120}}, {{"Ada", "eng", 120}});
    check("sorted", {{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}},
          {{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}});
    check("reversed", {{"Bob", "ops", 90}, {"Ada", "eng", 120}, {"Cy", "eng", 150}},
          {{"Cy", "eng", 150}, {"Ada", "eng", 120}, {"Bob", "ops", 90}});
    check("ties", {{"Zoe", "eng", 100}, {"Xi", "hr", 80}, {"Al", "eng", 100}, {"Yu", "hr", 80}, {"Mo", "eng", 100}},
          {{"Zoe", "eng", 100}, {"Al", "eng", 100}, {"Mo", "eng", 100}, {"Xi", "hr", 80}, {"Yu", "hr", 80}});
    List many_want = many(1);
    many_want.append_range(many(0));
    check("many-ties", many(-1), many_want);
    check("case", {{"Ann", "eng", 100}, {"Bea", "Sales", 90}, {"Cal", "ops", 80}},
          {{"Bea", "Sales", 90}, {"Ann", "eng", 100}, {"Cal", "ops", 80}});
    check("non-ascii", {{"Eve", "équipe", 100}, {"Fay", "zoo", 90}, {"Gus", "eng", 80}},
          {{"Gus", "eng", 80}, {"Fay", "zoo", 90}, {"Eve", "équipe", 100}});
    check("negative", {{"Hal", "eng", 0}, {"Ida", "eng", -5}, {"Jo", "eng", 5}},
          {{"Jo", "eng", 5}, {"Hal", "eng", 0}, {"Ida", "eng", -5}});
    check("overflow", {{"Kim", "eng", -2000000000}, {"Lee", "eng", 2000000000}},
          {{"Lee", "eng", 2000000000}, {"Kim", "eng", -2000000000}});
    return failures == 0 ? 0 : 1;
}

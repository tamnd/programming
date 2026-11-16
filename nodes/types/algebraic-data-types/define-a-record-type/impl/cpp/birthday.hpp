// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

#pragma once

#include <string>
#include <vector>

// An aggregate struct. The defaulted operator== compares the members in
// order.
struct Person {
    std::string name;
    int age;
    bool operator==(const Person &) const = default;
};

// birthday returns a new vector where each person called name is one year
// older.
std::vector<Person> birthday(const std::vector<Person> &people, const std::string &name);

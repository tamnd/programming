// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

#include "birthday.hpp"

std::vector<Person> birthday(const std::vector<Person> &people, const std::string &name) {
    std::vector<Person> out;
    out.reserve(people.size());
    for (Person p : people) { // p is a copy
        if (p.name == name) {
            p.age++;
        }
        out.push_back(p);
    }
    return out;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

#include "linked_list.hpp"

#include <algorithm>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<int> run_ops(const std::vector<std::string> &ops) {
    std::list<int> list;
    for (const auto &op : ops) {
        std::istringstream in(op);
        std::string name;
        int value = 0;
        in >> name;
        if (name == "push_front" && in >> value) {
            list.push_front(value);
        } else if (name == "push_back" && in >> value) {
            list.push_back(value);
        } else if (name == "pop_front" && !list.empty()) {
            list.pop_front();
        } else if (name == "pop_back" && !list.empty()) {
            list.pop_back();
        } else if (name == "remove" && in >> value) {
            // list.remove(value) would remove every item equal to value.
            auto it = std::ranges::find(list, value);
            if (it != list.end()) {
                list.erase(it);
            }
        } else {
            throw std::invalid_argument("bad operation: " + op);
        }
    }
    return {list.begin(), list.end()};
}

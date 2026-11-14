// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-linked-list/.

#include "linked_list.hpp"

#include <climits>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

static int failures = 0;

static void check(std::string_view name, const std::vector<std::string> &ops, const std::vector<int> &want) {
    if (run_ops(ops) != want) {
        std::cerr << "case " << name << " failed\n";
        failures++;
    }
}

int main() {
    check("deque", {"push_back 1", "push_back 2", "push_front 0", "pop_back", "push_back 3"}, std::vector<int>{0, 1, 3});
    check("remove-middle", {"push_back 1", "push_back 2", "push_back 3", "remove 2"}, std::vector<int>{1, 3});
    check("empty", {}, std::vector<int>{});
    check("push-pop", {"push_back 1", "pop_front"}, std::vector<int>{});
    check("single", {"push_front 7"}, std::vector<int>{7});
    check("remove-only", {"push_back 5", "remove 5", "push_back 6"}, std::vector<int>{6});
    check("remove-head", {"push_back 1", "push_back 2", "remove 1", "push_front 0"}, std::vector<int>{0, 2});
    check("remove-tail", {"push_back 1", "push_back 2", "remove 2", "push_back 3"}, std::vector<int>{1, 3});
    check("pop-both-ends", {"push_back 1", "push_back 2", "pop_front", "pop_back", "push_front 3"}, std::vector<int>{3});
    check("remove-absent", {"push_back 1", "remove 9"}, std::vector<int>{1});
    check("duplicates", {"push_back 4", "push_back 5", "push_back 4", "remove 4"}, std::vector<int>{5, 4});
    check("push-front-order", {"push_front 1", "push_front 2", "push_front 3"}, std::vector<int>{3, 2, 1});
    check("negative", {"push_front -1", "push_back -2", "remove -1"}, std::vector<int>{-2});
    check("limit", {"push_back 2147483647", "push_front -2147483648", "remove 2147483647", "push_back 2147483647"}, std::vector<int>{INT_MIN, INT_MAX});
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

#include "balanced.hpp"

#include <stack>

bool balanced(std::string_view text) {
    std::stack<char> stack;
    for (char c : text) {
        if (c == '(' || c == '[' || c == '{') {
            stack.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            char want = c == ')' ? '(' : c == ']' ? '[' : '{';
            // top() and pop() on an empty stack are undefined behavior, so
            // test empty() first.
            if (stack.empty() || stack.top() != want) {
                return false;
            }
            stack.pop(); // pop() returns nothing; read top() before it
        }
    }
    return stack.empty();
}

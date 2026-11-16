// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

#include "template.hpp"

namespace {

struct test_case {
    const char* name;
    const char* tmpl;
    std::vector<std::string> names;
    std::vector<std::string> values;
    const char* want;
};

const test_case cases[] = {
    {"greeting", "Hello, $name!", {"name"}, {"Ada"}, "Hello, Ada!"},
    {"braces", "${item}s cost $$${price}", {"item", "price"}, {"Apple", "3"}, "Apples cost $3"},
    {"two-uses", "$a and $a again", {"a"}, {"x"}, "x and x again"},
    {"no-placeholders", "plain text", {}, {}, "plain text"},
    {"empty-template", "", {"x"}, {"1"}, ""},
    {"dollar-escape", "$$name", {"name"}, {"Ada"}, "$name"},
    {"adjacent-text", "${n}th", {"n"}, {"4"}, "4th"},
    {"value-with-dollar", "$v", {"v"}, {"$x and ${y}"}, "$x and ${y}"},
    {"empty-value", "[$v]", {"v"}, {""}, "[]"},
    {"underscore-name", "$first_name $last_name", {"first_name", "last_name"}, {"Grace", "Hopper"}, "Grace Hopper"},
    {"name-ends-at-punct", "$a.$b", {"a", "b"}, {"1", "2"}, "1.2"},
    {"unused-value", "$a", {"a", "b"}, {"1", "2"}, "1"},
};

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        const std::string got = substitute(c.tmpl, c.names, c.values);
        if (got != c.want) {
            std::printf("FAIL %s: got \"%s\"\n", c.name, got.c_str());
            ++failures;
        }
    }
    try {
        substitute("Hi $who", {"name"}, {"Ada"});
        std::printf("FAIL unknown name\n");
        ++failures;
    } catch (const std::out_of_range&) {
    }
    if (failures != 0) {
        return 1;
    }
    std::printf("ok\n");
    return 0;
}

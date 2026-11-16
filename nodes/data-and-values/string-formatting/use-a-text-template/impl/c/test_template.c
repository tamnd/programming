// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

#include "template.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    const char *template;
    const char *names[4];
    const char *values[4];
    size_t count;
    const char *want;
};

static const struct test_case cases[] = {
    {"greeting", "Hello, $name!", {"name"}, {"Ada"}, 1, "Hello, Ada!"},
    {"braces", "${item}s cost $$${price}", {"item", "price"}, {"Apple", "3"}, 2, "Apples cost $3"},
    {"two-uses", "$a and $a again", {"a"}, {"x"}, 1, "x and x again"},
    {"no-placeholders", "plain text", {}, {}, 0, "plain text"},
    {"empty-template", "", {"x"}, {"1"}, 1, ""},
    {"dollar-escape", "$$name", {"name"}, {"Ada"}, 1, "$name"},
    {"adjacent-text", "${n}th", {"n"}, {"4"}, 1, "4th"},
    {"value-with-dollar", "$v", {"v"}, {"$x and ${y}"}, 1, "$x and ${y}"},
    {"empty-value", "[$v]", {"v"}, {""}, 1, "[]"},
    {"underscore-name", "$first_name $last_name", {"first_name", "last_name"}, {"Grace", "Hopper"}, 2, "Grace Hopper"},
    {"name-ends-at-punct", "$a.$b", {"a", "b"}, {"1", "2"}, 2, "1.2"},
    {"unused-value", "$a", {"a", "b"}, {"1", "2"}, 2, "1"},
};

int main(void) {
    int failures = 0;
    char buf[64];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int n = substitute(buf, sizeof buf, c->template, c->names, c->values, c->count);
        if (n != (int)strlen(c->want) || strcmp(buf, c->want) != 0) {
            printf("FAIL %s: got %d \"%s\"\n", c->name, n, buf);
            failures++;
        }
    }
    const char *const names[] = {"name"};
    const char *const values[] = {"Ada"};
    if (substitute(buf, sizeof buf, "Hi $who", names, values, 1) != -1) {
        printf("FAIL unknown name\n");
        failures++;
    }
    char small[6];
    if (substitute(small, sizeof small, "Hello, $name!", names, values, 1) != 11 || strcmp(small, "Hello") != 0) {
        printf("FAIL small buffer\n");
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}

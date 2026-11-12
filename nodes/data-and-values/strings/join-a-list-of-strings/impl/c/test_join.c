// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

#include "join.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN(a) (sizeof(a) / sizeof((a)[0]))

static int failures = 0;

static void check(const char *name, char *got, const char *want) {
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got [%s], want [%s]\n", name, got ? got : "NULL", want);
        failures++;
    }
    free(got);
}

int main(void) {
    const char *comma[] = {"a", "b", "c"};
    const char *colors[] = {"red", "green", "blue"};
    const char *words[] = {"Hello", "world"};
    const char *single[] = {"only"};
    const char *empties[] = {"", "", ""};
    const char *middle[] = {"a", "", "b"};
    const char *xy[] = {"x", "y"};
    const char *non_ascii[] = {"na\xc3\xafve", "\xe6\x97\xa5\xe6\x9c\xac"};
    const char *ab[] = {"a", "b"};
    const char *sep_in[] = {"a,b", "c"};

    check("comma", join(comma, LEN(comma), ","), "a,b,c");
    check("comma-space", join(colors, LEN(colors), ", "), "red, green, blue");
    check("words", join(words, LEN(words), " "), "Hello world");
    check("empty-list", join(NULL, 0, ","), "");
    check("single", join(single, LEN(single), ","), "only");
    check("empty-items", join(empties, LEN(empties), ","), ",,");
    check("empty-middle", join(middle, LEN(middle), "-"), "a--b");
    check("empty-sep", join(comma, LEN(comma), ""), "abc");
    check("long-sep", join(xy, LEN(xy), " -> "), "x -> y");
    // U+00B7 MIDDLE DOT is C2 B7 in UTF-8.
    check("non-ascii", join(non_ascii, LEN(non_ascii), "\xc2\xb7"), "na\xc3\xafve\xc2\xb7\xe6\x97\xa5\xe6\x9c\xac");
    // U+1F642 is F0 9F 99 82 in UTF-8.
    check("astral-sep", join(ab, LEN(ab), "\xf0\x9f\x99\x82"), "a\xf0\x9f\x99\x82" "b");
    check("sep-in-item", join(sep_in, LEN(sep_in), ","), "a,b,c");

    const int32_t numbers[] = {1, 2, 3};
    const int32_t negative[] = {-1, 0, 1};
    const int32_t answer[] = {42};
    const int32_t ends[] = {INT32_MAX, INT32_MIN};
    check("numbers", join_numbers(numbers, LEN(numbers), ","), "1,2,3");
    check("negative", join_numbers(negative, LEN(negative), " "), "-1 0 1");
    check("empty", join_numbers(NULL, 0, ","), "");
    check("single-number", join_numbers(answer, LEN(answer), ","), "42");
    check("int32-ends", join_numbers(ends, LEN(ends), ";"), "2147483647;-2147483648");

    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}

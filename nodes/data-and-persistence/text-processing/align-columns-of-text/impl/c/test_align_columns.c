// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

#include "align_columns.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char rosetta[] =
    "Given$a$text$file$of$many$lines,$where$fields$within$a$line$\n"
    "are$delineated$by$a$single$'dollar'$character,$write$a$program\n"
    "that$aligns$each$column$of$fields$by$ensuring$that$words$in$each$\n"
    "column$are$separated$by$at$least$one$space.\n"
    "Further,$allow$for$each$word$in$a$column$to$be$either$left$\n"
    "justified,$right$justified,$or$center$justified$within$its$column.\n"
;

struct test_case {
    const char *name, *text, *align, *want;
};

static const struct test_case cases[] = {
    {"rosetta-left", rosetta, "left", "Given      a          text       file   of     many      lines,     where    fields  within  a      line\n"
        "are        delineated by         a      single 'dollar'  character, write    a       program\n"
        "that       aligns     each       column of     fields    by         ensuring that    words   in     each\n"
        "column     are        separated  by     at     least     one        space.\n"
        "Further,   allow      for        each   word   in        a          column   to      be      either left\n"
        "justified, right      justified, or     center justified within     its      column.\n"},
    {"rosetta-right", rosetta, "right", "     Given          a       text   file     of      many     lines,    where  fields  within      a line\n"
        "       are delineated         by      a single  'dollar' character,    write       a program\n"
        "      that     aligns       each column     of    fields         by ensuring    that   words     in each\n"
        "    column        are  separated     by     at     least        one   space.\n"
        "  Further,      allow        for   each   word        in          a   column      to      be either left\n"
        "justified,      right justified,     or center justified     within      its column.\n"},
    {"rosetta-center", rosetta, "center", "  Given        a         text     file    of     many      lines,    where   fields  within    a    line\n"
        "   are     delineated     by       a    single 'dollar'  character,  write      a    program\n"
        "   that      aligns      each    column   of    fields       by     ensuring  that    words    in   each\n"
        "  column      are     separated    by     at     least      one      space.\n"
        " Further,    allow       for      each   word     in         a       column    to      be    either left\n"
        "justified,   right    justified,   or   center justified   within     its    column.\n"},
    {"two-lines", "a$bb$c\ndd$e$fff\n", "left", "a  bb c\ndd e  fff\n"},
    {"empty", "", "left", ""},
    {"empty-line", "a$b\n"
        "\n"
        "ccc$d\n", "left", "a   b\n"
        "\n"
        "ccc d\n"},
    {"empty-field", "a$$b\nc$dd$e\n", "left", "a    b\nc dd e\n"},
    {"trailing-dollar", "a$b$\ncc$d\n", "left", "a  b\ncc d\n"},
    {"ragged", "a$b$c\nddd\n", "right", "  a b c\nddd\n"},
    {"one-column", "a\n"
        "bbb\n"
        "cc\n", "right", "  a\n"
        "bbb\n"
        " cc\n"},
    {"no-final-newline", "a$b\ncc$d", "left", "a  b\ncc d\n"},
    {"center-odd", "a$b\nbbbb$c\n", "center", " a   b\nbbbb c\n"},
    {"center-last", "x$a\nx$bbbb\n", "center", "x  a\nx bbbb\n"},
    {"non-ascii", "caf\303\251$x\nab$y\n", "left", "caf\303\251 x\nab   y\n"},
    {"outside-the-bmp", "\360\237\230\200$x\nab$y\n", "left", "\360\237\230\200  x\nab y\n"},
    {"spaces-in-field", "a b$c\nd$e\n", "left", "a b c\nd   e\n"},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        char *got = align_columns(c->text, c->align);
        if (got == NULL || strcmp(got, c->want) != 0) {
            fprintf(stderr, "FAIL %s: got \"%s\"\n", c->name, got != NULL ? got : "(null)");
            failures++;
        }
        free(got);
    }

    // 100000 lines from 0$x to 99999$x.
    char *many = malloc(100000 * 8 + 1);
    size_t len = 0;
    for (int i = 0; i < 100000; i++) {
        len += (size_t)snprintf(many + len, 100000 * 8 + 1 - len, "%d$x\n", i);
    }
    char *got = align_columns(many, "right");
    if (got == NULL || strncmp(got, "    0 x\n", 8) != 0 || strcmp(got + strlen(got) - 8, "99999 x\n") != 0 ||
        strlen(got) != 100000 * 8) {
        fprintf(stderr, "FAIL many-lines\n");
        failures++;
    }
    free(got);
    free(many);

    errno = 0;
    if (align_columns("a$b\n", "justify") != NULL || errno != EINVAL) {
        fprintf(stderr, "FAIL bad-align\n");
        failures++;
    }
    if (failures == 0) {
        printf("ok\n");
    }
    return failures == 0 ? 0 : 1;
}

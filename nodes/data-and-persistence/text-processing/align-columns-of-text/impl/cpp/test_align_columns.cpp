// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

#include "align_columns.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

const std::string rosetta =
    "Given$a$text$file$of$many$lines,$where$fields$within$a$line$\n"
    "are$delineated$by$a$single$'dollar'$character,$write$a$program\n"
    "that$aligns$each$column$of$fields$by$ensuring$that$words$in$each$\n"
    "column$are$separated$by$at$least$one$space.\n"
    "Further,$allow$for$each$word$in$a$column$to$be$either$left$\n"
    "justified,$right$justified,$or$center$justified$within$its$column.\n"
;

struct Case {
    std::string name, text, align, want;
};

const Case cases[] = {
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

}  // namespace

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        std::string got = align_columns(c.text, c.align);
        if (got != c.want) {
            std::cerr << "FAIL " << c.name << ": got \"" << got << "\"\n";
            failures++;
        }
    }

    // 100000 lines from 0$x to 99999$x.
    std::string many;
    for (int i = 0; i < 100000; i++) {
        many += std::to_string(i) + "$x\n";
    }
    std::string got = align_columns(many, "right");
    if (got.size() != 100000 * 8 || !got.starts_with("    0 x\n") || !got.ends_with("99999 x\n")) {
        std::cerr << "FAIL many-lines\n";
        failures++;
    }

    try {
        align_columns("a$b\n", "justify");
        std::cerr << "FAIL bad-align: no exception\n";
        failures++;
    } catch (const std::invalid_argument&) {
    }
    if (failures == 0) {
        std::cout << "ok\n";
    }
    return failures == 0 ? 0 : 1;
}

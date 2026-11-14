// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

use algo_align_columns::{align_columns, Align, BadAlign};

const ROSETTA: &str = concat!(
    "Given$a$text$file$of$many$lines,$where$fields$within$a$line$\n",
    "are$delineated$by$a$single$'dollar'$character,$write$a$program\n",
    "that$aligns$each$column$of$fields$by$ensuring$that$words$in$each$\n",
    "column$are$separated$by$at$least$one$space.\n",
    "Further,$allow$for$each$word$in$a$column$to$be$either$left$\n",
    "justified,$right$justified,$or$center$justified$within$its$column.\n",
);

/// Each case is (name, text, align, result).
const CASES: &[(&str, &str, &str, &str)] = &[
    ("rosetta-left", ROSETTA, "left", concat!(
        "Given      a          text       file   of     many      lines,     where    fields  within  a      line\n",
        "are        delineated by         a      single 'dollar'  character, write    a       program\n",
        "that       aligns     each       column of     fields    by         ensuring that    words   in     each\n",
        "column     are        separated  by     at     least     one        space.\n",
        "Further,   allow      for        each   word   in        a          column   to      be      either left\n",
        "justified, right      justified, or     center justified within     its      column.\n",
    )),
    ("rosetta-right", ROSETTA, "right", concat!(
        "     Given          a       text   file     of      many     lines,    where  fields  within      a line\n",
        "       are delineated         by      a single  'dollar' character,    write       a program\n",
        "      that     aligns       each column     of    fields         by ensuring    that   words     in each\n",
        "    column        are  separated     by     at     least        one   space.\n",
        "  Further,      allow        for   each   word        in          a   column      to      be either left\n",
        "justified,      right justified,     or center justified     within      its column.\n",
    )),
    ("rosetta-center", ROSETTA, "center", concat!(
        "  Given        a         text     file    of     many      lines,    where   fields  within    a    line\n",
        "   are     delineated     by       a    single 'dollar'  character,  write      a    program\n",
        "   that      aligns      each    column   of    fields       by     ensuring  that    words    in   each\n",
        "  column      are     separated    by     at     least      one      space.\n",
        " Further,    allow       for      each   word     in         a       column    to      be    either left\n",
        "justified,   right    justified,   or   center justified   within     its    column.\n",
    )),
    ("two-lines", "a$bb$c\ndd$e$fff\n", "left", "a  bb c\ndd e  fff\n"),
    ("empty", "", "left", ""),
    ("empty-line", concat!(
        "a$b\n",
        "\n",
        "ccc$d\n",
    ), "left", concat!(
        "a   b\n",
        "\n",
        "ccc d\n",
    )),
    ("empty-field", "a$$b\nc$dd$e\n", "left", "a    b\nc dd e\n"),
    ("trailing-dollar", "a$b$\ncc$d\n", "left", "a  b\ncc d\n"),
    ("ragged", "a$b$c\nddd\n", "right", "  a b c\nddd\n"),
    ("one-column", concat!(
        "a\n",
        "bbb\n",
        "cc\n",
    ), "right", concat!(
        "  a\n",
        "bbb\n",
        " cc\n",
    )),
    ("no-final-newline", "a$b\ncc$d", "left", "a  b\ncc d\n"),
    ("center-odd", "a$b\nbbbb$c\n", "center", " a   b\nbbbb c\n"),
    ("center-last", "x$a\nx$bbbb\n", "center", "x  a\nx bbbb\n"),
    ("non-ascii", "caf\u{e9}$x\nab$y\n", "left", "caf\u{e9} x\nab   y\n"),
    ("outside-the-bmp", "\u{1f600}$x\nab$y\n", "left", "\u{1f600}  x\nab y\n"),
    ("spaces-in-field", "a b$c\nd$e\n", "left", "a b c\nd   e\n"),
];

#[test]
fn cases() {
    for &(name, text, align, want) in CASES {
        let align: Align = align.parse().unwrap();
        assert_eq!(align_columns(text, align), want, "{name}");
    }
}

#[test]
fn many_lines() {
    let text: String = (0..100000).map(|i| format!("{i}$x\n")).collect();
    let got = align_columns(&text, Align::Right);
    let lines: Vec<&str> = got.split('\n').collect();
    assert_eq!(lines.len(), 100001);
    assert_eq!(lines[0], "    0 x");
    assert_eq!(lines[99999], "99999 x");
}

#[test]
fn bad_align() {
    assert_eq!(
        "justify".parse::<Align>(),
        Err(BadAlign("justify".to_string()))
    );
}

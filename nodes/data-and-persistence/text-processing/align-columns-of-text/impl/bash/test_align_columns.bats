#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/align-columns-of-text/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/align_columns.sh"
  # The first example of the Rosetta Code task, with no final LF.
  rosetta='Given$a$text$file$of$many$lines,$where$fields$within$a$line$
are$delineated$by$a$single$'\''dollar'\''$character,$write$a$program
that$aligns$each$column$of$fields$by$ensuring$that$words$in$each$
column$are$separated$by$at$least$one$space.
Further,$allow$for$each$word$in$a$column$to$be$either$left$
justified,$right$justified,$or$center$justified$within$its$column.'
}

check() { # check TEXT ALIGN WANT, with printf %b escapes in TEXT and WANT
  # The x keeps a final LF, which $(...) removes.
  [ "$(printf '%b' "$1" | align_columns "$2"; printf x)" = "$(printf '%bx' "$3")" ]
}

@test "two-lines" { check 'a$bb$c\ndd$e$fff\n' left 'a  bb c\ndd e  fff\n'; }
@test "empty" { check '' left ''; }
@test "empty-line" { check 'a$b\n\nccc$d\n' left 'a   b\n\nccc d\n'; }
@test "empty-field" { check 'a$$b\nc$dd$e\n' left 'a    b\nc dd e\n'; }
@test "trailing-dollar" { check 'a$b$\ncc$d\n' left 'a  b\ncc d\n'; }
@test "ragged" { check 'a$b$c\nddd\n' right '  a b c\nddd\n'; }
@test "one-column" { check 'a\nbbb\ncc\n' right '  a\nbbb\n cc\n'; }
@test "no-final-newline" { check 'a$b\ncc$d' left 'a  b\ncc d\n'; }
@test "center-odd" { check 'a$b\nbbbb$c\n' center ' a   b\nbbbb c\n'; }
@test "center-last" { check 'x$a\nx$bbbb\n' center 'x  a\nx bbbb\n'; }
@test "non-ascii" { check 'caf\xc3\xa9$x\nab$y\n' left 'caf\xc3\xa9 x\nab   y\n'; }
@test "outside-the-bmp" { check '\xf0\x9f\x98\x80$x\nab$y\n' left '\xf0\x9f\x98\x80  x\nab y\n'; }
@test "spaces-in-field" { check 'a b$c\nd$e\n' left 'a b c\nd   e\n'; }

@test "rosetta-left" {
  want='Given      a          text       file   of     many      lines,     where    fields  within  a      line
are        delineated by         a      single '\''dollar'\''  character, write    a       program
that       aligns     each       column of     fields    by         ensuring that    words   in     each
column     are        separated  by     at     least     one        space.
Further,   allow      for        each   word   in        a          column   to      be      either left
justified, right      justified, or     center justified within     its      column.'
  check "$rosetta\n" left "$want\n"
}

@test "rosetta-right" {
  want='     Given          a       text   file     of      many     lines,    where  fields  within      a line
       are delineated         by      a single  '\''dollar'\'' character,    write       a program
      that     aligns       each column     of    fields         by ensuring    that   words     in each
    column        are  separated     by     at     least        one   space.
  Further,      allow        for   each   word        in          a   column      to      be either left
justified,      right justified,     or center justified     within      its column.'
  check "$rosetta\n" right "$want\n"
}

@test "rosetta-center" {
  want='  Given        a         text     file    of     many      lines,    where   fields  within    a    line
   are     delineated     by       a    single '\''dollar'\''  character,  write      a    program
   that      aligns      each    column   of    fields       by     ensuring  that    words    in   each
  column      are     separated    by     at     least      one      space.
 Further,    allow       for      each   word     in         a       column    to      be    either left
justified,   right    justified,   or   center justified   within     its    column.'
  check "$rosetta\n" center "$want\n"
}

@test "many-lines" {
  out="$BATS_TEST_TMPDIR/out.txt"
  seq 0 99999 | sed 's/$/$x/' | align_columns right >"$out"
  [ "$(wc -l <"$out")" -eq 100000 ]
  [ "$(head -n 1 "$out")" = "    0 x" ]
  [ "$(tail -n 1 "$out")" = "99999 x" ]
}

@test "bad-align" {
  run align_columns justify </dev/null
  [ "$status" -eq 2 ]
}

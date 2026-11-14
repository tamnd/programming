// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

package aligncolumns

import (
	"errors"
	"fmt"
	"strings"
	"testing"
)

const rosetta = "Given$a$text$file$of$many$lines,$where$fields$within$a$line$\n" +
	"are$delineated$by$a$single$'dollar'$character,$write$a$program\n" +
	"that$aligns$each$column$of$fields$by$ensuring$that$words$in$each$\n" +
	"column$are$separated$by$at$least$one$space.\n" +
	"Further,$allow$for$each$word$in$a$column$to$be$either$left$\n" +
	"justified,$right$justified,$or$center$justified$within$its$column.\n"

var cases = []struct{ name, text, align, want string }{
	{"rosetta-left", rosetta, "left", "Given      a          text       file   of     many      lines,     where    fields  within  a      line\n" +
		"are        delineated by         a      single 'dollar'  character, write    a       program\n" +
		"that       aligns     each       column of     fields    by         ensuring that    words   in     each\n" +
		"column     are        separated  by     at     least     one        space.\n" +
		"Further,   allow      for        each   word   in        a          column   to      be      either left\n" +
		"justified, right      justified, or     center justified within     its      column.\n"},
	{"rosetta-right", rosetta, "right", "     Given          a       text   file     of      many     lines,    where  fields  within      a line\n" +
		"       are delineated         by      a single  'dollar' character,    write       a program\n" +
		"      that     aligns       each column     of    fields         by ensuring    that   words     in each\n" +
		"    column        are  separated     by     at     least        one   space.\n" +
		"  Further,      allow        for   each   word        in          a   column      to      be either left\n" +
		"justified,      right justified,     or center justified     within      its column.\n"},
	{"rosetta-center", rosetta, "center", "  Given        a         text     file    of     many      lines,    where   fields  within    a    line\n" +
		"   are     delineated     by       a    single 'dollar'  character,  write      a    program\n" +
		"   that      aligns      each    column   of    fields       by     ensuring  that    words    in   each\n" +
		"  column      are     separated    by     at     least      one      space.\n" +
		" Further,    allow       for      each   word     in         a       column    to      be    either left\n" +
		"justified,   right    justified,   or   center justified   within     its    column.\n"},
	{"two-lines", "a$bb$c\ndd$e$fff\n", "left", "a  bb c\ndd e  fff\n"},
	{"empty", "", "left", ""},
	{"empty-line", "a$b\n" +
		"\n" +
		"ccc$d\n", "left", "a   b\n" +
		"\n" +
		"ccc d\n"},
	{"empty-field", "a$$b\nc$dd$e\n", "left", "a    b\nc dd e\n"},
	{"trailing-dollar", "a$b$\ncc$d\n", "left", "a  b\ncc d\n"},
	{"ragged", "a$b$c\nddd\n", "right", "  a b c\nddd\n"},
	{"one-column", "a\n" +
		"bbb\n" +
		"cc\n", "right", "  a\n" +
		"bbb\n" +
		" cc\n"},
	{"no-final-newline", "a$b\ncc$d", "left", "a  b\ncc d\n"},
	{"center-odd", "a$b\nbbbb$c\n", "center", " a   b\nbbbb c\n"},
	{"center-last", "x$a\nx$bbbb\n", "center", "x  a\nx bbbb\n"},
	{"non-ascii", "caf\u00e9$x\nab$y\n", "left", "caf\u00e9 x\nab   y\n"},
	{"outside-the-bmp", "\U0001f600$x\nab$y\n", "left", "\U0001f600  x\nab y\n"},
	{"spaces-in-field", "a b$c\nd$e\n", "left", "a b c\nd   e\n"},
}

func TestAlignColumns(t *testing.T) {
	for _, c := range cases {
		got, err := AlignColumns(c.text, c.align)
		if err != nil || got != c.want {
			t.Errorf("%s: got %q, %v; want %q", c.name, got, err, c.want)
		}
	}
}

func TestManyLines(t *testing.T) {
	var b strings.Builder
	for i := range 100000 {
		fmt.Fprintf(&b, "%d$x\n", i)
	}
	got, err := AlignColumns(b.String(), "right")
	if err != nil {
		t.Fatal(err)
	}
	lines := strings.Split(got, "\n")
	if len(lines) != 100001 || lines[0] != "    0 x" || lines[99999] != "99999 x" {
		t.Errorf("got %d lines, %q, %q", len(lines), lines[0], lines[99999])
	}
}

func TestBadAlign(t *testing.T) {
	if _, err := AlignColumns("a$b\n", "justify"); !errors.Is(err, ErrBadAlign) {
		t.Errorf("got %v, want ErrBadAlign", err)
	}
}

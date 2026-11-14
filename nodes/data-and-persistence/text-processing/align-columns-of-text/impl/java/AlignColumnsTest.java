// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class AlignColumnsTest {
  static final String ROSETTA =
      "Given$a$text$file$of$many$lines,$where$fields$within$a$line$\n" +
          "are$delineated$by$a$single$'dollar'$character,$write$a$program\n" +
          "that$aligns$each$column$of$fields$by$ensuring$that$words$in$each$\n" +
          "column$are$separated$by$at$least$one$space.\n" +
          "Further,$allow$for$each$word$in$a$column$to$be$either$left$\n" +
          "justified,$right$justified,$or$center$justified$within$its$column.\n";

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("rosetta-left", ROSETTA, "left", "Given      a          text       file   of     many      lines,     where    fields  within  a      line\n" +
            "are        delineated by         a      single 'dollar'  character, write    a       program\n" +
            "that       aligns     each       column of     fields    by         ensuring that    words   in     each\n" +
            "column     are        separated  by     at     least     one        space.\n" +
            "Further,   allow      for        each   word   in        a          column   to      be      either left\n" +
            "justified, right      justified, or     center justified within     its      column.\n"),
        Arguments.of("rosetta-right", ROSETTA, "right", "     Given          a       text   file     of      many     lines,    where  fields  within      a line\n" +
            "       are delineated         by      a single  'dollar' character,    write       a program\n" +
            "      that     aligns       each column     of    fields         by ensuring    that   words     in each\n" +
            "    column        are  separated     by     at     least        one   space.\n" +
            "  Further,      allow        for   each   word        in          a   column      to      be either left\n" +
            "justified,      right justified,     or center justified     within      its column.\n"),
        Arguments.of("rosetta-center", ROSETTA, "center", "  Given        a         text     file    of     many      lines,    where   fields  within    a    line\n" +
            "   are     delineated     by       a    single 'dollar'  character,  write      a    program\n" +
            "   that      aligns      each    column   of    fields       by     ensuring  that    words    in   each\n" +
            "  column      are     separated    by     at     least      one      space.\n" +
            " Further,    allow       for      each   word     in         a       column    to      be    either left\n" +
            "justified,   right    justified,   or   center justified   within     its    column.\n"),
        Arguments.of("two-lines", "a$bb$c\ndd$e$fff\n", "left", "a  bb c\ndd e  fff\n"),
        Arguments.of("empty", "", "left", ""),
        Arguments.of("empty-line", "a$b\n" +
            "\n" +
            "ccc$d\n", "left", "a   b\n" +
            "\n" +
            "ccc d\n"),
        Arguments.of("empty-field", "a$$b\nc$dd$e\n", "left", "a    b\nc dd e\n"),
        Arguments.of("trailing-dollar", "a$b$\ncc$d\n", "left", "a  b\ncc d\n"),
        Arguments.of("ragged", "a$b$c\nddd\n", "right", "  a b c\nddd\n"),
        Arguments.of("one-column", "a\n" +
            "bbb\n" +
            "cc\n", "right", "  a\n" +
            "bbb\n" +
            " cc\n"),
        Arguments.of("no-final-newline", "a$b\ncc$d", "left", "a  b\ncc d\n"),
        Arguments.of("center-odd", "a$b\nbbbb$c\n", "center", " a   b\nbbbb c\n"),
        Arguments.of("center-last", "x$a\nx$bbbb\n", "center", "x  a\nx bbbb\n"),
        Arguments.of("non-ascii", "caf\u00e9$x\nab$y\n", "left", "caf\u00e9 x\nab   y\n"),
        Arguments.of("outside-the-bmp", "\ud83d\ude00$x\nab$y\n", "left", "\ud83d\ude00  x\nab y\n"),
        Arguments.of("spaces-in-field", "a b$c\nd$e\n", "left", "a b c\nd   e\n"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void alignsTheColumns(String name, String text, String align, String want) {
    assertEquals(want, AlignColumns.align(text, align));
  }

  @Test
  void manyLines() {
    StringBuilder text = new StringBuilder();
    for (int i = 0; i < 100000; i++) {
      text.append(i).append("$x\n");
    }
    String[] lines = AlignColumns.align(text.toString(), "right").split("\n", -1);
    assertEquals(100001, lines.length);
    assertEquals("    0 x", lines[0]);
    assertEquals("99999 x", lines[99999]);
  }

  @Test
  void badAlign() {
    assertThrows(IllegalArgumentException.class, () -> AlignColumns.align("a$b\n", "justify"));
  }
}

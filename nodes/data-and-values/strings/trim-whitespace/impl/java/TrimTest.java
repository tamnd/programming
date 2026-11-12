// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TrimTest {
  // The white space is the 6 ASCII characters: space, tab, line feed, vertical tab, form feed and carriage return. U+00A0, U+3000, U+0085 and U+FEFF stay.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("both-ends", "  hello  ", "hello"),
        Arguments.of("tab-newline", "\thello world\n", "hello world"),
        Arguments.of("none", "hello", "hello"),
        Arguments.of("empty", "", ""),
        Arguments.of("all-space", " \t\n\r ", ""),
        Arguments.of("vt-ff", "\u000b\fx\f\u000b", "x"),
        Arguments.of("other-control", "\u0001x\u0001", "\u0001x\u0001"),
        Arguments.of("no-break-space", "\u00a0x\u00a0", "\u00a0x\u00a0"),
        Arguments.of("ideographic-space", "\u3000x\u3000", "\u3000x\u3000"),
        Arguments.of("next-line", "\u0085x\u0085", "\u0085x\u0085"),
        Arguments.of("bom", "\ufeffx", "\ufeffx"),
        Arguments.of("astral", " \ud83d\ude42 ", "\ud83d\ude42"),
        Arguments.of("combining", " \u0301x ", "\u0301x"));
  }

  static Stream<Arguments> start() {
    return Stream.of(
        Arguments.of("start-both-ends", "  hello  ", "hello  "),
        Arguments.of("start-tab-newline", "\n\tx", "x"),
        Arguments.of("start-empty", "", ""),
        Arguments.of("start-all-space", "   ", ""),
        Arguments.of("start-no-break-space", "\u00a0 x", "\u00a0 x"),
        Arguments.of("start-end-only", "x \t", "x \t"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void trim(String name, String s, String want) {
    assertEquals(want, Trim.trim(s));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("start")
  void trimStart(String name, String s, String want) {
    assertEquals(want, Trim.trimStart(s));
  }
}

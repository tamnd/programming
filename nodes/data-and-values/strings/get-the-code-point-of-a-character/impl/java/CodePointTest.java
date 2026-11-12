// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-code-point-of-a-character/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CodePointTest {
  // U+1F642 is one code point, but it is 2 UTF-16 code units and 4 UTF-8 bytes. U+10FFFF is the largest code point.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("upper", "A", 65),
        Arguments.of("lower", "a", 97),
        Arguments.of("digit", "0", 48),
        Arguments.of("space", " ", 32),
        Arguments.of("tab", "\t", 9),
        Arguments.of("tilde", "~", 126),
        Arguments.of("two-byte", "\u00e9", 233),
        Arguments.of("three-byte", "\u20ac", 8364),
        Arguments.of("cjk", "\u65e5", 26085),
        Arguments.of("combining", "\u0301", 769),
        Arguments.of("astral", "\ud83d\ude42", 128578),
        Arguments.of("max", "\udbff\udfff", 1114111));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void codePoint(String name, String c, int want) {
    assertEquals(want, CodePoint.codePoint(c));
  }
}

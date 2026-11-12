// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CaseTest {
  // Only the ASCII letters change. U+0131 is the dotless i, U+0130 is the capital I with a dot, U+212A is the KELVIN SIGN, and U+FF41 is the fullwidth a.
  static Stream<Arguments> upper() {
    return Stream.of(
        Arguments.of("word", "hello", "HELLO"),
        Arguments.of("mixed", "Hello, World!", "HELLO, WORLD!"),
        Arguments.of("already", "ABC", "ABC"),
        Arguments.of("empty", "", ""),
        Arguments.of("digits-punct", "a1-b2_c3", "A1-B2_C3"),
        Arguments.of("non-ascii", "caf\u00e9", "CAF\u00e9"),
        Arguments.of("sharp-s", "stra\u00dfe", "STRA\u00dfE"),
        Arguments.of("dotless-i", "\u0131 i", "\u0131 I"),
        Arguments.of("greek", "\u03c3", "\u03c3"),
        Arguments.of("fullwidth", "\uff41", "\uff41"),
        Arguments.of("astral", "a\ud83d\ude42b", "A\ud83d\ude42B"),
        Arguments.of("combining", "cafe\u0301", "CAFE\u0301"));
  }

  static Stream<Arguments> lower() {
    return Stream.of(
        Arguments.of("lower-word", "HELLO", "hello"),
        Arguments.of("lower-mixed", "Hello, World!", "hello, world!"),
        Arguments.of("lower-empty", "", ""),
        Arguments.of("lower-dotted-i", "\u0130", "\u0130"),
        Arguments.of("lower-sigma", "\u03a3\u0391\u03a3", "\u03a3\u0391\u03a3"),
        Arguments.of("lower-kelvin", "\u212a", "\u212a"),
        Arguments.of("lower-astral", "A\ud83d\ude42B", "a\ud83d\ude42b"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("upper")
  void toUpper(String name, String s, String want) {
    assertEquals(want, Case.toUpper(s));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("lower")
  void toLower(String name, String s, String want) {
    assertEquals(want, Case.toLower(s));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CompareTest {
  // U+FF61 is HALFWIDTH IDEOGRAPHIC FULL STOP. It is before U+1F642 in code point order, but its UTF-16 code unit is after the high surrogate of U+1F642.
  static Stream<Arguments> order() {
    return Stream.of(
        Arguments.of("less", "apple", "banana", -1),
        Arguments.of("equal", "apple", "apple", 0),
        Arguments.of("greater", "pear", "apple", 1),
        Arguments.of("both-empty", "", "", 0),
        Arguments.of("empty-first", "", "a", -1),
        Arguments.of("prefix", "app", "apple", -1),
        Arguments.of("prefix-longer", "apple", "app", 1),
        Arguments.of("upper-before-lower", "Zebra", "apple", -1),
        Arguments.of("lower-after-upper", "a", "A", 1),
        Arguments.of("digits", "10", "9", -1),
        Arguments.of("non-ascii", "\u00e9", "z", 1),
        Arguments.of("astral-after-bmp", "\uff61", "\ud83d\ude42", -1),
        Arguments.of("astral", "\ud83d\ude42", "\ud83d\ude43", -1),
        Arguments.of("combining", "cafe\u0301", "caf\u00e9", -1));
  }

  static Stream<Arguments> equal() {
    return Stream.of(
        Arguments.of("eq-same", "abc", "abc", true),
        Arguments.of("eq-case", "abc", "ABC", false),
        Arguments.of("eq-empty", "", "", true),
        Arguments.of("eq-combining", "cafe\u0301", "caf\u00e9", false),
        Arguments.of("eq-astral", "\ud83d\ude42", "\ud83d\ude42", true),
        Arguments.of("eq-trailing-space", "abc", "abc ", false));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("order")
  void compare(String name, String a, String b, int want) {
    assertEquals(want, Compare.compare(a, b));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("equal")
  void equals(String name, String a, String b, boolean want) {
    assertEquals(want, Compare.equals(a, b));
  }
}

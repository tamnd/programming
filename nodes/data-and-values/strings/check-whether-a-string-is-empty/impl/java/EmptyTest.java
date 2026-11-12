// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class EmptyTest {
  // U+00A0 is NO-BREAK SPACE, U+200B is ZERO WIDTH SPACE, and U+3000 is IDEOGRAPHIC SPACE. None of them is ASCII white space.
  static Stream<Arguments> empty() {
    return Stream.of(
        Arguments.of("letter", "a", false),
        Arguments.of("word", "hello", false),
        Arguments.of("empty", "", true),
        Arguments.of("space", " ", false),
        Arguments.of("newline", "\n", false),
        Arguments.of("nbsp", "\u00a0", false),
        Arguments.of("zero-width", "\u200b", false),
        Arguments.of("combining", "\u0301", false),
        Arguments.of("astral", "\ud83d\ude42", false),
        Arguments.of("cjk", "日", false));
  }

  static Stream<Arguments> blank() {
    return Stream.of(
        Arguments.of("blank-empty", "", true),
        Arguments.of("blank-space", " ", true),
        Arguments.of("blank-mixed", " \t\n\r", true),
        Arguments.of("blank-vt-ff", "\u000b\f", true),
        Arguments.of("blank-letter", "a", false),
        Arguments.of("blank-padded", "  a  ", false),
        Arguments.of("blank-nbsp", "\u00a0", false),
        Arguments.of("blank-zero-width", "\u200b", false),
        Arguments.of("blank-ideographic", "\u3000", false),
        Arguments.of("blank-astral", " \ud83d\ude42 ", false));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("empty")
  void isEmpty(String name, String s, boolean want) {
    assertEquals(want, Empty.isEmpty(s));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("blank")
  void isBlank(String name, String s, boolean want) {
    assertEquals(want, Empty.isBlank(s));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReverseStringTest {
  static Stream<Arguments> codePointCases() {
    return Stream.of(
        Arguments.of("ascii", "hello", "olleh"),
        Arguments.of("two-words", "abc def", "fed cba"),
        Arguments.of("palindrome", "racecar", "racecar"),
        Arguments.of("empty", "", ""),
        Arguments.of("single", "a", "a"),
        Arguments.of("two-byte", "naïve", "evïan"),
        Arguments.of("three-byte", "日本語", "語本日"),
        Arguments.of("astral", "a🙂b", "b🙂a"),
        Arguments.of("two-astral", "🙂😀", "😀🙂"),
        Arguments.of("combining-mark", "e\u0301x", "x\u0301e"),
        Arguments.of("flag", "🇫🇷", "🇷🇫"));
  }

  static Stream<Arguments> graphemeCases() {
    return Stream.of(
        Arguments.of("ascii", "abc", "cba"),
        Arguments.of("empty", "", ""),
        Arguments.of("combining-mark", "e\u0301x", "xe\u0301"),
        Arguments.of("flags", "🇫🇷🇩🇪", "🇩🇪🇫🇷"),
        Arguments.of("zwj-family", "a👨\u200d👩\u200d👧b", "b👨\u200d👩\u200d👧a"),
        Arguments.of("skin-tone", "👍🏽!", "!👍🏽"),
        Arguments.of("crlf", "a\r\nb", "b\r\na"),
        Arguments.of("hangul-jamo", "\u1100\u1161x", "x\u1100\u1161"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("codePointCases")
  void reversesCodePoints(String name, String s, String want) {
    assertEquals(want, ReverseString.reverseCodePoints(s), name);
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("graphemeCases")
  void reversesGraphemes(String name, String s, String want) {
    assertEquals(want, ReverseString.reverseGraphemes(s), name);
  }
}

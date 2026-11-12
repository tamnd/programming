// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class StringLengthTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("ascii", "hello", new int[] {5, 5, 5}),
        Arguments.of("words", "Hello, world", new int[] {12, 12, 12}),
        Arguments.of("two-byte", "naïve", new int[] {6, 5, 5}),
        Arguments.of("three-byte", "日本語", new int[] {9, 3, 3}),
        Arguments.of("cyrillic", "Привет", new int[] {12, 6, 6}),
        Arguments.of("empty", "", new int[] {0, 0, 0}),
        Arguments.of("single", "a", new int[] {1, 1, 1}),
        Arguments.of("euro-sign", "€", new int[] {3, 1, 1}),
        Arguments.of("astral", "🙂", new int[] {4, 2, 1}),
        Arguments.of("mixed", "a🙂b", new int[] {6, 4, 3}),
        Arguments.of("combining-mark", "e\u0301", new int[] {3, 2, 2}),
        Arguments.of("precomposed", "é", new int[] {2, 1, 1}),
        Arguments.of("flag", "🇫🇷", new int[] {8, 4, 2}),
        Arguments.of("zwj-family", "👨\u200d👩\u200d👧", new int[] {18, 8, 5}),
        Arguments.of("nul", "a\u0000b", new int[] {3, 3, 3}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void lengths(String name, String s, int[] want) {
    assertArrayEquals(want, StringLength.lengths(s), name);
  }
}

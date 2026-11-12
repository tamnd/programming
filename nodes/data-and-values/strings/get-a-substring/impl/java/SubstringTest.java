// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SubstringTest {
  // U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("middle", "Hello, world", 7, 12, "world"),
        Arguments.of("prefix", "Hello, world", 0, 5, "Hello"),
        Arguments.of("one-char", "abc", 1, 2, "b"),
        Arguments.of("empty", "", 0, 0, ""),
        Arguments.of("whole", "abc", 0, 3, "abc"),
        Arguments.of("empty-range", "abc", 1, 1, ""),
        Arguments.of("at-end", "abc", 3, 3, ""),
        Arguments.of("non-ascii", "naïve café", 2, 5, "ïve"),
        Arguments.of("cjk", "日本語テキスト", 3, 7, "テキスト"),
        Arguments.of("astral", "a\uD83D\uDE42b\uD83D\uDE43c", 1, 4, "\uD83D\uDE42b\uD83D\uDE43"),
        Arguments.of("astral-only", "\uD83D\uDE42\uD83D\uDE43", 1, 2, "\uD83D\uDE43"),
        Arguments.of("combining", "e\u0301t\u00e9", 0, 1, "e"),
        Arguments.of("combining-mark", "e\u0301t\u00e9", 1, 3, "\u0301t"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void substring(String name, String s, int start, int end, String want) {
    assertEquals(want, Substring.substring(s, start, end));
  }
}

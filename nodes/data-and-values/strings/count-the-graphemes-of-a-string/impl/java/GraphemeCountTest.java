// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-graphemes-of-a-string/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class GraphemeCountTest {
  // A flag is 2 regional indicators, and U+1F469 U+200D U+1F4BB is 3 code points. Each of them is one grapheme.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("empty", "", 0),
        Arguments.of("ascii", "abc", 3),
        Arguments.of("precomposed", "\u00e9", 1),
        Arguments.of("emoji", "\ud83d\ude42", 1),
        Arguments.of("combining", "e\u0301", 1),
        Arguments.of("two-marks", "e\u0301\u0302", 1),
        Arguments.of("leading-mark", "\u0301a", 2),
        Arguments.of("crlf", "\r\n", 1),
        Arguments.of("lfcr", "\n\r", 2),
        Arguments.of("cr-mark", "\r\u0301", 2),
        Arguments.of("soft-hyphen", "a\u00adb", 3),
        Arguments.of("flag", "\ud83c\uddeb\ud83c\uddf7", 1),
        Arguments.of("three-indicators", "\ud83c\uddeb\ud83c\uddf7\ud83c\uddea", 2),
        Arguments.of("two-flags", "\ud83c\uddeb\ud83c\uddf7\ud83c\uddea\ud83c\uddf8", 2),
        Arguments.of("zwj-sequence", "\ud83d\udc69\u200d\ud83d\udcbb", 1),
        Arguments.of("zwj-after-letter", "a\u200d\ud83d\ude42", 2),
        Arguments.of("skin-tone", "\ud83d\udc4d\ud83c\udffd", 1),
        Arguments.of("variation-selector", "\u2764\ufe0f", 1));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void countGraphemes(String name, String s, int want) {
    assertEquals(want, GraphemeCount.countGraphemes(s));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SearchTest {
  // U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
  static Stream<Arguments> first() {
    return Stream.of(
        Arguments.of("found", "Hello, world", "world", 7),
        Arguments.of("first-of-two", "abcabc", "bc", 1),
        Arguments.of("not-found", "abc", "d", -1),
        Arguments.of("empty-sub", "abc", "", 0),
        Arguments.of("empty-s", "", "a", -1),
        Arguments.of("both-empty", "", "", 0),
        Arguments.of("longer-sub", "ab", "abc", -1),
        Arguments.of("overlap", "aaaa", "aa", 0),
        Arguments.of("case-sensitive", "Hello", "hello", -1),
        Arguments.of("non-ascii", "naïve caf\u00e9", "caf\u00e9", 6),
        Arguments.of("cjk", "日本語テキスト", "テキ", 3),
        Arguments.of("astral", "a\ud83d\ude42b\ud83d\ude42c", "b", 2),
        Arguments.of("astral-sub", "x\ud83d\ude42y", "\ud83d\ude42y", 1),
        Arguments.of("combining", "cafe\u0301", "e", 3),
        Arguments.of("precomposed", "cafe\u0301", "\u00e9", -1));
  }

  static Stream<Arguments> last() {
    return Stream.of(
        Arguments.of("last-of-two", "abcabc", "bc", 4),
        Arguments.of("last-not-found", "abc", "d", -1),
        Arguments.of("last-empty-sub", "abc", "", 3),
        Arguments.of("last-empty-s", "", "", 0),
        Arguments.of("last-overlap", "aaaa", "aa", 2),
        Arguments.of("last-astral", "\ud83d\ude42a\ud83d\ude42a", "a", 3),
        Arguments.of("last-non-ascii", "caf\u00e9 caf\u00e9", "\u00e9", 8));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("first")
  void find(String name, String s, String sub, int want) {
    assertEquals(want, Search.find(s, sub));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("last")
  void findLast(String name, String s, String sub, int want) {
    assertEquals(want, Search.findLast(s, sub));
  }
}

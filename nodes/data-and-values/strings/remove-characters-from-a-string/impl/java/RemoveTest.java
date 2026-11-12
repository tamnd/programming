// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class RemoveTest {
  // A character is a code point. U+1F642 and U+1F643 have the same high surrogate in UTF-16, and U+00E0 and U+00E9 have the same first byte in UTF-8.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("one-char", "banana", "a", "bnn"),
        Arguments.of("set", "hello, world", "lo", "he, wrd"),
        Arguments.of("punct", "a-b_c.d", "-_.", "abcd"),
        Arguments.of("empty", "", "abc", ""),
        Arguments.of("empty-set", "hello", "", "hello"),
        Arguments.of("none-found", "hello", "xyz", "hello"),
        Arguments.of("all", "aaa", "a", ""),
        Arguments.of("adjacent", "aab", "a", "b"),
        Arguments.of("astral", "a\ud83d\ude42b\ud83d\ude42", "\ud83d\ude42", "ab"),
        Arguments.of("astral-other", "\ud83d\ude42\ud83d\ude43", "\ud83d\ude43", "\ud83d\ude42"),
        Arguments.of("non-ascii", "\u00e0\u00e9", "\u00e9", "\u00e0"),
        Arguments.of("combining-mark", "e\u0301x", "\u0301", "ex"),
        Arguments.of("combining-base", "e\u0301", "e", "\u0301"),
        Arguments.of("composed", "e\u0301", "\u00e9", "e\u0301"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void removeChars(String name, String s, String chars, String want) {
    assertEquals(want, Remove.removeChars(s, chars));
  }
}

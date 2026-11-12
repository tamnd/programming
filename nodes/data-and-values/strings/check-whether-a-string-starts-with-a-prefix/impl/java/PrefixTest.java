// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PrefixTest {
  // U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
  static Stream<Arguments> starts() {
    return Stream.of(
        Arguments.of("yes", "Hello, world", "Hello", true),
        Arguments.of("no", "Hello, world", "world", false),
        Arguments.of("whole", "abc", "abc", true),
        Arguments.of("empty-prefix", "abc", "", true),
        Arguments.of("empty-s", "", "a", false),
        Arguments.of("both-empty", "", "", true),
        Arguments.of("longer-prefix", "ab", "abc", false),
        Arguments.of("case-sensitive", "Hello", "hello", false),
        Arguments.of("non-ascii", "caf\u00e9 au lait", "caf\u00e9", true),
        Arguments.of("cjk", "日本語", "日本", true),
        Arguments.of("astral", "\ud83d\ude42\ud83d\ude43", "\ud83d\ude42", true),
        Arguments.of("astral-no", "\ud83d\ude42", "\ud83d\ude43", false),
        Arguments.of("combining", "cafe\u0301", "cafe", true),
        Arguments.of("precomposed", "caf\u00e9", "cafe", false));
  }

  static Stream<Arguments> ends() {
    return Stream.of(
        Arguments.of("ends-yes", "report.txt", ".txt", true),
        Arguments.of("ends-no", "report.txt", ".csv", false),
        Arguments.of("ends-empty", "abc", "", true),
        Arguments.of("ends-longer", "txt", "a.txt", false),
        Arguments.of("ends-case", "A.TXT", ".txt", false),
        Arguments.of("ends-astral", "a\ud83d\ude42", "\ud83d\ude42", true),
        Arguments.of("ends-combining", "cafe\u0301", "e", false));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("starts")
  void startsWith(String name, String s, String prefix, boolean want) {
    assertEquals(want, Prefix.startsWith(s, prefix));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("ends")
  void endsWith(String name, String s, String suffix, boolean want) {
    assertEquals(want, Prefix.endsWith(s, suffix));
  }
}

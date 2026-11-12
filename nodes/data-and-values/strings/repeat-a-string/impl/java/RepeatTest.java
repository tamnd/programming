// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class RepeatTest {
  // The case many has the 200 chars of 100 copies of ab in one literal.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("word", "ab", 3, "ababab"),
        Arguments.of("one", "abc", 1, "abc"),
        Arguments.of("dash", "-", 5, "-----"),
        Arguments.of("zero", "abc", 0, ""),
        Arguments.of("empty", "", 5, ""),
        Arguments.of("empty-zero", "", 0, ""),
        Arguments.of("many", "ab", 100, "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab"),
        Arguments.of("astral", "\ud83d\ude42", 3, "\ud83d\ude42\ud83d\ude42\ud83d\ude42"),
        Arguments.of("combining", "e\u0301", 2, "e\u0301e\u0301"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void repeat(String name, String s, int n, String want) {
    assertEquals(want, Repeat.repeat(s, n));
  }
}

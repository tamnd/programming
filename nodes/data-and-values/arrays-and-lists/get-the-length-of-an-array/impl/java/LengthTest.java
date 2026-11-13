// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class LengthTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three", new String[] {"a", "b", "c"}, 3),
        Arguments.of("two-words", new String[] {"apple", "banana"}, 2),
        Arguments.of("empty", new String[] {}, 0),
        Arguments.of("single", new String[] {"x"}, 1),
        Arguments.of("empty-strings", new String[] {"", ""}, 2),
        Arguments.of("non-ascii", new String[] {"é", "日本語"}, 2),
        Arguments.of("duplicates", new String[] {"a", "a", "a"}, 3),
        Arguments.of("ten", new String[] {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, 10));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void length(String name, String[] items, int want) {
    assertEquals(want, Length.length(items));
  }
}

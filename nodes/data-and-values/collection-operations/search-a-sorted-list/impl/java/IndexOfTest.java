// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class IndexOfTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("middle", new int[] {1, 3, 5, 7, 9}, 5, 2),
        Arguments.of("first", new int[] {1, 3, 5, 7, 9}, 1, 0),
        Arguments.of("last", new int[] {1, 3, 5, 7, 9}, 9, 4),
        Arguments.of("missing", new int[] {1, 3, 5, 7, 9}, 4, -1),
        Arguments.of("below", new int[] {1, 3, 5}, 0, -1),
        Arguments.of("above", new int[] {1, 3, 5}, 6, -1),
        Arguments.of("empty", new int[] {}, 1, -1),
        Arguments.of("single-hit", new int[] {7}, 7, 0),
        Arguments.of("single-miss", new int[] {7}, 8, -1),
        Arguments.of("duplicates", new int[] {1, 2, 2, 2, 3}, 2, 1),
        Arguments.of("all-same", new int[] {4, 4, 4, 4}, 4, 0),
        Arguments.of("negative", new int[] {-9, -5, -1, 0}, -5, 1),
        Arguments.of("extremes", new int[] {Integer.MIN_VALUE, 0, 2147483647}, 2147483647, 2),
        Arguments.of("min-target", new int[] {Integer.MIN_VALUE, 0, 2147483647}, Integer.MIN_VALUE, 0));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void indexOf(String name, int[] items, int target, int want) {
    assertEquals(want, IndexOf.indexOf(items, target));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class IndexOfTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("middle", new int[] {4, 7, 1}, 7, 1),
        Arguments.of("absent", new int[] {4, 7, 1}, 9, -1),
        Arguments.of("empty", new int[] {}, 0, -1),
        Arguments.of("single", new int[] {5}, 5, 0),
        Arguments.of("single-absent", new int[] {5}, 0, -1),
        Arguments.of("first", new int[] {7, 8, 9}, 7, 0),
        Arguments.of("last", new int[] {7, 8, 9}, 9, 2),
        Arguments.of("duplicates", new int[] {3, 5, 3, 5}, 5, 1),
        Arguments.of("all-same", new int[] {2, 2, 2}, 2, 0),
        Arguments.of("negative", new int[] {0, -1, -2}, -2, 2),
        Arguments.of("limit", new int[] {2147483647, Integer.MIN_VALUE}, Integer.MIN_VALUE, 1),
        Arguments.of("limit-absent", new int[] {2147483647}, Integer.MIN_VALUE, -1));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void indexOf(String name, int[] items, int value, int want) {
    int[] items0 = items.clone();
    assertEquals(want, IndexOf.indexOf(items, value));
    assertArrayEquals(items0, items);
  }
}

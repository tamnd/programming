// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CountItemsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("mixed", new int[] {3, 1, 3, 2, 1, 3}, new int[][] {{1, 2}, {2, 1}, {3, 3}}),
        Arguments.of("same", new int[] {5, 5, 5}, new int[][] {{5, 3}}),
        Arguments.of("empty", new int[] {}, new int[][] {}),
        Arguments.of("single", new int[] {7}, new int[][] {{7, 1}}),
        Arguments.of("negative", new int[] {-1, 2, -1, 0}, new int[][] {{-1, 2}, {0, 1}, {2, 1}}),
        Arguments.of("distinct", new int[] {4, 2, 9}, new int[][] {{2, 1}, {4, 1}, {9, 1}}),
        Arguments.of("order", new int[] {9, 8, 9}, new int[][] {{8, 1}, {9, 2}}),
        Arguments.of("zero", new int[] {0, 0}, new int[][] {{0, 2}}),
        Arguments.of("extremes", new int[] {2147483647, -2147483648, 2147483647}, new int[][] {{-2147483648, 1}, {2147483647, 2}}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void countItems(String name, int[] items, int[][] want) {
    // assertArrayEquals compares the inner arrays by their items.
    assertArrayEquals(want, CountItems.countItems(items));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/filter-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class KeepOddTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("mixed", new int[] {1, 2, 3, 4, 5}, new int[] {1, 3, 5}),
        Arguments.of("tens", new int[] {10, 15, 20, 25}, new int[] {15, 25}),
        Arguments.of("empty", new int[] {}, new int[] {}),
        Arguments.of("none-odd", new int[] {2, 4, 6}, new int[] {}),
        Arguments.of("all-odd", new int[] {1, 3, 5}, new int[] {1, 3, 5}),
        Arguments.of("negative", new int[] {-3, -2, -1, 0}, new int[] {-3, -1}),
        Arguments.of("zero", new int[] {0}, new int[] {}),
        Arguments.of("order", new int[] {9, 1, 7}, new int[] {9, 1, 7}),
        Arguments.of("extremes", new int[] {-2147483648, 2147483647}, new int[] {2147483647}),
        Arguments.of("duplicates", new int[] {3, 3, 2, 3}, new int[] {3, 3, 3}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void keepOdd(String name, int[] items, int[] want) {
    assertArrayEquals(want, KeepOdd.keepOdd(items));
  }
}

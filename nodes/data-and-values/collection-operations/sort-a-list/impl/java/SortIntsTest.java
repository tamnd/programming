// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SortIntsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("small", new int[] {3, 1, 2}, new int[] {1, 2, 3}),
        Arguments.of("five", new int[] {5, 4, 1, 3, 2}, new int[] {1, 2, 3, 4, 5}),
        Arguments.of("empty", new int[] {}, new int[] {}),
        Arguments.of("single", new int[] {1}, new int[] {1}),
        Arguments.of("sorted", new int[] {1, 2, 3}, new int[] {1, 2, 3}),
        Arguments.of("reversed", new int[] {9, 8, 7}, new int[] {7, 8, 9}),
        Arguments.of("duplicates", new int[] {2, 1, 2, 1}, new int[] {1, 1, 2, 2}),
        Arguments.of("negative", new int[] {-1, -10, 5}, new int[] {-10, -1, 5}),
        Arguments.of("digits", new int[] {10, 9, 100, 1}, new int[] {1, 9, 10, 100}),
        Arguments.of("extremes", new int[] {2147483647, -2147483648, 0}, new int[] {-2147483648, 0, 2147483647}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void sortInts(String name, int[] items, int[] want) {
    assertArrayEquals(want, SortInts.sortInts(items));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CombinationsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("two-of-four", new int[] {1, 2, 3, 4}, 2, new int[][] {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}),
        Arguments.of("three-of-four", new int[] {1, 2, 3, 4}, 3, new int[][] {{1, 2, 3}, {1, 2, 4}, {1, 3, 4}, {2, 3, 4}}),
        Arguments.of("zero-k", new int[] {1, 2}, 0, new int[][] {{}}),
        Arguments.of("empty", new int[] {}, 0, new int[][] {{}}),
        Arguments.of("k-too-large", new int[] {1, 2}, 3, new int[][] {}),
        Arguments.of("all", new int[] {1, 2, 3}, 3, new int[][] {{1, 2, 3}}),
        Arguments.of("duplicates", new int[] {1, 1, 2}, 2, new int[][] {{1, 1}, {1, 2}, {1, 2}}),
        Arguments.of("unsorted", new int[] {3, 1, 2}, 2, new int[][] {{3, 1}, {3, 2}, {1, 2}}),
        Arguments.of("extremes", new int[] {Integer.MIN_VALUE, Integer.MAX_VALUE}, 1, new int[][] {{Integer.MIN_VALUE}, {Integer.MAX_VALUE}}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void combinations(String name, int[] items, int k, int[][] want) {
    assertArrayEquals(want, Combinations.combinations(items, k), name);
  }
}

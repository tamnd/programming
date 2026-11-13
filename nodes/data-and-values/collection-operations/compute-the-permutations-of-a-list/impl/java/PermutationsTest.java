// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PermutationsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three", new int[] {1, 2, 3}, new int[][] {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}}),
        Arguments.of("two", new int[] {5, 6}, new int[][] {{5, 6}, {6, 5}}),
        Arguments.of("empty", new int[] {}, new int[][] {{}}),
        Arguments.of("single", new int[] {7}, new int[][] {{7}}),
        Arguments.of("duplicates", new int[] {1, 1, 2}, new int[][] {{1, 1, 2}, {1, 2, 1}, {1, 1, 2}, {1, 2, 1}, {2, 1, 1}, {2, 1, 1}}),
        Arguments.of("unsorted", new int[] {3, 1, 2}, new int[][] {{3, 1, 2}, {3, 2, 1}, {1, 3, 2}, {1, 2, 3}, {2, 3, 1}, {2, 1, 3}}),
        Arguments.of("extremes", new int[] {Integer.MIN_VALUE, Integer.MAX_VALUE}, new int[][] {{Integer.MIN_VALUE, Integer.MAX_VALUE}, {Integer.MAX_VALUE, Integer.MIN_VALUE}}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void permutations(String name, int[] items, int[][] want) {
    assertArrayEquals(want, Permutations.permutations(items), name);
  }
}

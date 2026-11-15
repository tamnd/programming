// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FindInGridTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("middle", new int[][] {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 5, new int[] {1, 1}),
        Arguments.of("first-twenty", new int[][] {{3, 8, 12}, {5, 20, 20}, {20, 1, 4}}, 20, new int[] {1, 1}),
        Arguments.of("first-cell", new int[][] {{9, 1}, {2, 3}}, 9, new int[] {0, 0}),
        Arguments.of("last-cell", new int[][] {{1, 2}, {3, 9}}, 9, new int[] {1, 1}),
        Arguments.of("row-major", new int[][] {{1, 2, 9}, {9, 3, 4}}, 9, new int[] {0, 2}),
        Arguments.of("missing", new int[][] {{1, 2}, {3, 4}}, 9, new int[] {-1, -1}),
        Arguments.of("empty-grid", new int[][] {}, 1, new int[] {-1, -1}),
        Arguments.of("empty-rows", new int[][] {{}, {}, {5}}, 5, new int[] {2, 0}),
        Arguments.of("ragged", new int[][] {{1}, {2, 3, 4}, {5, 6}}, 6, new int[] {2, 1}),
        Arguments.of("negative", new int[][] {{-1, -2}, {-3, -4}}, -3, new int[] {1, 0}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void findInGrid(String name, int[][] grid, int target, int[] want) {
    assertArrayEquals(want, FindInGrid.findInGrid(grid, target), name);
  }
}

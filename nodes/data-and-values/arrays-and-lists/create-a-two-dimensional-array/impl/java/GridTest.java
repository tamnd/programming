// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.Arrays;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class GridTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("two-by-three", 2, 3, 0, new int[][] {{0, 0, 0}, {0, 0, 0}}),
        Arguments.of("three-by-one", 3, 1, 7, new int[][] {{7}, {7}, {7}}),
        Arguments.of("no-rows", 0, 3, 1, new int[][] {}),
        Arguments.of("no-cols", 2, 0, 1, new int[][] {{}, {}}),
        Arguments.of("one-cell", 1, 1, 5, new int[][] {{5}}),
        Arguments.of("one-row", 1, 4, 2, new int[][] {{2, 2, 2, 2}}),
        Arguments.of("one-col", 4, 1, 2, new int[][] {{2}, {2}, {2}, {2}}),
        Arguments.of("negative", 2, 2, -3, new int[][] {{-3, -3}, {-3, -3}}),
        Arguments.of("min-int", 2, 2, Integer.MIN_VALUE, new int[][] {{Integer.MIN_VALUE, Integer.MIN_VALUE}, {Integer.MIN_VALUE, Integer.MIN_VALUE}}),
        Arguments.of("max-int", 1, 2, 2147483647, new int[][] {{2147483647, 2147483647}}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void grid(String name, int rows, int cols, int value, int[][] want) {
    int[][] got = Grid.grid(rows, cols, value);
    assertTrue(Arrays.deepEquals(want, got), name);
    if (rows >= 2 && cols >= 1) {
      got[0][0] = value ^ 1;
      assertEquals(value, got[1][0]);
    }
  }
}

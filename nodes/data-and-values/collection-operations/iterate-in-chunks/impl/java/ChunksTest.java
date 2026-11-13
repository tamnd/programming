// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-in-chunks/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ChunksTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("pairs", new int[] {1, 2, 3, 4, 5}, 2, new int[][] {{1, 2}, {3, 4}, {5}}),
        Arguments.of("triples", new int[] {1, 2, 3, 4, 5, 6}, 3, new int[][] {{1, 2, 3}, {4, 5, 6}}),
        Arguments.of("empty", new int[] {}, 3, new int[][] {}),
        Arguments.of("size-one", new int[] {7, 8, 9}, 1, new int[][] {{7}, {8}, {9}}),
        Arguments.of("size-larger", new int[] {1, 2}, 5, new int[][] {{1, 2}}),
        Arguments.of("size-equal", new int[] {1, 2, 3}, 3, new int[][] {{1, 2, 3}}),
        Arguments.of("last-shorter", new int[] {1, 2, 3, 4}, 3, new int[][] {{1, 2, 3}, {4}}),
        Arguments.of("negative", new int[] {-1, -2, -3}, 2, new int[][] {{-1, -2}, {-3}}),
        Arguments.of("extremes", new int[] {-2147483648, 2147483647, 0}, 2, new int[][] {{-2147483648, 2147483647}, {0}}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void chunks(String name, int[] items, int size, int[][] want) {
    // assertArrayEquals compares the inner arrays by their items.
    assertArrayEquals(want, Chunks.chunks(items, size));
  }
}

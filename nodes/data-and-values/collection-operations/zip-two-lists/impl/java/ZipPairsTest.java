// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/zip-two-lists/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ZipPairsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("same-length", new int[] {1, 2, 3}, new int[] {4, 5, 6}, new int[][] {{1, 4}, {2, 5}, {3, 6}}),
        Arguments.of("tens", new int[] {10, 20}, new int[] {30, 40}, new int[][] {{10, 30}, {20, 40}}),
        Arguments.of("empty", new int[] {}, new int[] {}, new int[][] {}),
        Arguments.of("first-empty", new int[] {}, new int[] {1, 2}, new int[][] {}),
        Arguments.of("second-empty", new int[] {1, 2}, new int[] {}, new int[][] {}),
        Arguments.of("first-shorter", new int[] {1}, new int[] {7, 8}, new int[][] {{1, 7}}),
        Arguments.of("second-shorter", new int[] {1, 2, 3}, new int[] {9}, new int[][] {{1, 9}}),
        Arguments.of("negative", new int[] {-1, -2}, new int[] {3, -4}, new int[][] {{-1, 3}, {-2, -4}}),
        Arguments.of("extremes", new int[] {-2147483648, 2147483647}, new int[] {2147483647, -2147483648}, new int[][] {{-2147483648, 2147483647}, {2147483647, -2147483648}}),
        Arguments.of("duplicates", new int[] {5, 5}, new int[] {5, 5}, new int[][] {{5, 5}, {5, 5}}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void zipPairs(String name, int[] a, int[] b, int[][] want) {
    // assertArrayEquals compares the inner arrays by their items.
    assertArrayEquals(want, ZipPairs.zipPairs(a, b));
  }
}

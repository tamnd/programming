// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CartesianProductTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("two", new int[][] {{1, 2}, {3, 4}}, new int[][] {{1, 3}, {1, 4}, {2, 3}, {2, 4}}),
        Arguments.of("three", new int[][] {{1, 2}, {3}, {4, 5}}, new int[][] {{1, 3, 4}, {1, 3, 5}, {2, 3, 4}, {2, 3, 5}}),
        Arguments.of("no-lists", new int[][] {}, new int[][] {{}}),
        Arguments.of("one-empty", new int[][] {{1, 2}, {}}, new int[][] {}),
        Arguments.of("single", new int[][] {{7, 8, 9}}, new int[][] {{7}, {8}, {9}}),
        Arguments.of("duplicates", new int[][] {{1, 1}, {2}}, new int[][] {{1, 2}, {1, 2}}),
        Arguments.of("negative", new int[][] {{-1}, {-2, -3}}, new int[][] {{-1, -2}, {-1, -3}}),
        Arguments.of("extremes", new int[][] {{Integer.MIN_VALUE, Integer.MAX_VALUE}, {0}}, new int[][] {{Integer.MIN_VALUE, 0}, {Integer.MAX_VALUE, 0}}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void cartesianProduct(String name, int[][] lists, int[][] want) {
    assertArrayEquals(want, CartesianProduct.cartesianProduct(lists), name);
  }
}

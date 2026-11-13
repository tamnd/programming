// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FlattenTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three", new int[][] {new int[] {1, 2}, new int[] {3}, new int[] {4, 5, 6}}, new int[] {1, 2, 3, 4, 5, 6}),
        Arguments.of("two", new int[][] {new int[] {7}, new int[] {8, 9}}, new int[] {7, 8, 9}),
        Arguments.of("empty", new int[][] {}, new int[] {}),
        Arguments.of("empty-inner", new int[][] {new int[] {}, new int[] {1}, new int[] {}}, new int[] {1}),
        Arguments.of("all-empty", new int[][] {new int[] {}, new int[] {}}, new int[] {}),
        Arguments.of("single", new int[][] {new int[] {5}}, new int[] {5}),
        Arguments.of("one-list", new int[][] {new int[] {1, 2, 3}}, new int[] {1, 2, 3}),
        Arguments.of("negative", new int[][] {new int[] {-1, -2}, new int[] {3}}, new int[] {-1, -2, 3}),
        Arguments.of("extremes", new int[][] {new int[] {Integer.MIN_VALUE}, new int[] {2147483647, 0}}, new int[] {Integer.MIN_VALUE, 2147483647, 0}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void flatten(String name, int[][] lists, int[] want) {
    assertArrayEquals(want, Flatten.flatten(lists), name);
  }
}

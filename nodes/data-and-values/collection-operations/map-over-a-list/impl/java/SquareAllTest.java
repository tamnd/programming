// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/map-over-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SquareAllTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("small", new int[] {1, 2, 3}, new int[] {1, 4, 9}),
        Arguments.of("tens", new int[] {5, 10}, new int[] {25, 100}),
        Arguments.of("empty", new int[] {}, new int[] {}),
        Arguments.of("single", new int[] {7}, new int[] {49}),
        Arguments.of("negative", new int[] {-3, -1}, new int[] {9, 1}),
        Arguments.of("zero", new int[] {0}, new int[] {0}),
        Arguments.of("limit", new int[] {46340, -46340}, new int[] {2147395600, 2147395600}),
        Arguments.of("order", new int[] {3, 1, 2}, new int[] {9, 1, 4}),
        Arguments.of("duplicates", new int[] {2, 2}, new int[] {4, 4}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void squareAll(String name, int[] items, int[] want) {
    assertArrayEquals(want, SquareAll.squareAll(items));
  }
}

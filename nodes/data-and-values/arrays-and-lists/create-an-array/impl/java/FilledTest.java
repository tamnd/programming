// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FilledTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three-sevens", 3, 7, new int[] {7, 7, 7}),
        Arguments.of("five-zeros", 5, 0, new int[] {0, 0, 0, 0, 0}),
        Arguments.of("empty", 0, 7, new int[] {}),
        Arguments.of("single", 1, 42, new int[] {42}),
        Arguments.of("zero", 2, 0, new int[] {0, 0}),
        Arguments.of("negative", 2, -1, new int[] {-1, -1}),
        Arguments.of("max-int", 2, 2147483647, new int[] {2147483647, 2147483647}),
        Arguments.of("min-int", 2, Integer.MIN_VALUE, new int[] {Integer.MIN_VALUE, Integer.MIN_VALUE}),
        Arguments.of("ten", 10, 1, new int[] {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void filled(String name, int n, int value, int[] want) {
    assertArrayEquals(want, Filled.filled(n, value));
  }
}

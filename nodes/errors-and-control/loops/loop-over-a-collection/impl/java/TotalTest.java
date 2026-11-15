// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-a-collection/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TotalTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three", new int[] {1, 2, 3}, 6L),
        Arguments.of("mixed", new int[] {10, -4, 7, 0, 3}, 16L),
        Arguments.of("empty", new int[] {}, 0L),
        Arguments.of("single", new int[] {7}, 7L),
        Arguments.of("negative", new int[] {-5, 2, -1}, -4L),
        Arguments.of("limit", new int[] {2147483647, 2147483647, 2}, 4294967296L),
        Arguments.of("limit-low", new int[] {Integer.MIN_VALUE, Integer.MIN_VALUE}, -4294967296L));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void total(String name, int[] items, long want) {
    assertEquals(want, Total.total(items), name);
  }
}

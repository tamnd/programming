// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TotalTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("small", new int[] {1, 2, 3, 4}, 10),
        Arguments.of("mixed", new int[] {10, -3, 5}, 12),
        Arguments.of("empty", new int[] {}, 0),
        Arguments.of("single", new int[] {7}, 7),
        Arguments.of("negative", new int[] {-1, -2, -3}, -6),
        Arguments.of("zero-sum", new int[] {5, -5}, 0),
        Arguments.of("max", new int[] {2147483647}, 2147483647),
        Arguments.of("min", new int[] {-2147483648}, -2147483648),
        Arguments.of("to-the-limit", new int[] {2147483646, 1}, 2147483647),
        Arguments.of("partial-overflow", new int[] {2147483647, 1, -1}, 2147483647),
        Arguments.of("partial-underflow", new int[] {-2147483648, -1, 1}, -2147483648));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void total(String name, int[] items, int want) {
    assertEquals(want, Total.total(items));
  }
}

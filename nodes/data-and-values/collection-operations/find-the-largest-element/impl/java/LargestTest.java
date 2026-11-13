// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-largest-element/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.NoSuchElementException;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class LargestTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("digits", new int[] {3, 1, 4, 1, 5}, 5),
        Arguments.of("middle", new int[] {10, 20, 15}, 20),
        Arguments.of("single", new int[] {7}, 7),
        Arguments.of("all-negative", new int[] {-3, -1, -2}, -1),
        Arguments.of("first", new int[] {9, 2, 3}, 9),
        Arguments.of("last", new int[] {1, 2, 9}, 9),
        Arguments.of("duplicates", new int[] {5, 9, 9, 1}, 9),
        Arguments.of("all-same", new int[] {4, 4, 4}, 4),
        Arguments.of("zero", new int[] {-5, 0, -1}, 0),
        Arguments.of("extremes", new int[] {-2147483648, 2147483647, 0}, 2147483647),
        Arguments.of("min-only", new int[] {-2147483648}, -2147483648));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void largest(String name, int[] items, int want) {
    assertEquals(want, Largest.largest(items));
  }

  @Test
  void emptyThrows() {
    assertThrows(NoSuchElementException.class, () -> Largest.largest(new int[0]));
  }
}

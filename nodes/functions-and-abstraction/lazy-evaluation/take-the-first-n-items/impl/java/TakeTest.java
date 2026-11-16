// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/take-the-first-n-items/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TakeTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("first-three", new int[] {1, 2, 3, 4, 5}, 3, new int[] {1, 2, 3}),
        Arguments.of("first-two", new int[] {10, 20, 30, 40}, 2, new int[] {10, 20}),
        Arguments.of("zero", new int[] {1, 2}, 0, new int[] {}),
        Arguments.of("empty", new int[] {}, 3, new int[] {}),
        Arguments.of("more-than-length", new int[] {1, 2}, 5, new int[] {1, 2}),
        Arguments.of("equal-length", new int[] {4, 5, 6}, 3, new int[] {4, 5, 6}),
        Arguments.of("one", new int[] {9, 8}, 1, new int[] {9}),
        Arguments.of("negative", new int[] {-1, -2, -3}, 2, new int[] {-1, -2}),
        Arguments.of("extremes", new int[] {-2147483648, 2147483647, 0}, 2, new int[] {-2147483648, 2147483647}));
  }

  static Stream<Arguments> infinite() {
    return Stream.of(
        Arguments.of("from-zero", 0, 5, new int[] {0, 1, 2, 3, 4}),
        Arguments.of("from-ten", 10, 3, new int[] {10, 11, 12}),
        Arguments.of("none", 7, 0, new int[] {}),
        Arguments.of("one", 0, 1, new int[] {0}),
        Arguments.of("large-start", 999998, 2, new int[] {999998, 999999}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void take(String name, int[] items, int n, int[] want) {
    assertArrayEquals(want, Take.take(items, n));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("infinite")
  void takeNaturals(String name, int start, int n, int[] want) {
    assertArrayEquals(want, Take.takeNaturals(start, n));
  }

  @Test
  void takeReturnsACopy() {
    int[] items = {1, 2, 3};
    Take.take(items, 2)[0] = 99;
    assertArrayEquals(new int[] {1, 2, 3}, items);
  }
}

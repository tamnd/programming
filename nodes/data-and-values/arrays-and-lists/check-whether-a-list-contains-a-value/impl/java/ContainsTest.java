// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ContainsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("present", new int[] {1, 2, 3}, 2, true),
        Arguments.of("absent", new int[] {1, 2, 3}, 4, false),
        Arguments.of("empty", new int[] {}, 0, false),
        Arguments.of("single-yes", new int[] {5}, 5, true),
        Arguments.of("single-no", new int[] {5}, 0, false),
        Arguments.of("first", new int[] {7, 8, 9}, 7, true),
        Arguments.of("last", new int[] {7, 8, 9}, 9, true),
        Arguments.of("duplicates", new int[] {2, 2, 2}, 2, true),
        Arguments.of("negative", new int[] {-1, -2, -3}, -2, true),
        Arguments.of("zero-absent", new int[] {-1, 1}, 0, false),
        Arguments.of("limit", new int[] {2147483647, Integer.MIN_VALUE}, Integer.MIN_VALUE, true),
        Arguments.of("limit-absent", new int[] {2147483647}, Integer.MIN_VALUE, false));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void contains(String name, int[] items, int value, boolean want) {
    int[] items0 = items.clone();
    assertEquals(want, Contains.contains(items, value));
    assertArrayEquals(items0, items);
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReverseListTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three", new int[] {1, 2, 3}, new int[] {3, 2, 1}),
        Arguments.of("four", new int[] {4, 7, 1, 9}, new int[] {9, 1, 7, 4}),
        Arguments.of("empty", new int[] {}, new int[] {}),
        Arguments.of("single", new int[] {5}, new int[] {5}),
        Arguments.of("two", new int[] {1, 2}, new int[] {2, 1}),
        Arguments.of("duplicates", new int[] {1, 1, 2}, new int[] {2, 1, 1}),
        Arguments.of("palindrome", new int[] {1, 2, 1}, new int[] {1, 2, 1}),
        Arguments.of("negative", new int[] {-1, 0, -3}, new int[] {-3, 0, -1}),
        Arguments.of("limit", new int[] {2147483647, Integer.MIN_VALUE, 0}, new int[] {0, Integer.MIN_VALUE, 2147483647}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void reverse(String name, int[] items, int[] want) {
    int[] items0 = items.clone();
    int[] got = ReverseList.reverse(items);
    assertArrayEquals(want, got);
    assertNotSame(items, got);
    assertArrayEquals(items0, items);
  }
}

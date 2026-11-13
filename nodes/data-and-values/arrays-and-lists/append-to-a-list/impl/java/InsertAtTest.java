// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class InsertAtTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("at-end", new int[] {1, 2}, 2, 3, new int[] {1, 2, 3}),
        Arguments.of("at-front", new int[] {2, 3}, 0, 1, new int[] {1, 2, 3}),
        Arguments.of("in-middle", new int[] {1, 3}, 1, 2, new int[] {1, 2, 3}),
        Arguments.of("empty", new int[] {}, 0, 7, new int[] {7}),
        Arguments.of("single-front", new int[] {5}, 0, 4, new int[] {4, 5}),
        Arguments.of("single-end", new int[] {5}, 1, 6, new int[] {5, 6}),
        Arguments.of("end-of-many", new int[] {1, 2, 3, 4}, 4, 5, new int[] {1, 2, 3, 4, 5}),
        Arguments.of("front-of-many", new int[] {1, 2, 3, 4}, 0, 0, new int[] {0, 1, 2, 3, 4}),
        Arguments.of("duplicates", new int[] {1, 1}, 1, 1, new int[] {1, 1, 1}),
        Arguments.of("limit", new int[] {2147483647}, 1, Integer.MIN_VALUE, new int[] {2147483647, Integer.MIN_VALUE}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void insertAt(String name, int[] items, int index, int value, int[] want) {
    int[] items0 = items.clone();
    assertArrayEquals(want, InsertAt.insertAt(items, index, value));
    assertArrayEquals(items0, items);
  }

  @Test
  void indexPastTheEnd() {
    assertThrows(IndexOutOfBoundsException.class, () -> InsertAt.insertAt(new int[] {1, 2}, 3, 9));
  }
}

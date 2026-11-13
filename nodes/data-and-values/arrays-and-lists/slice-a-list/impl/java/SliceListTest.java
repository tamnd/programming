// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/slice-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SliceListTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("middle", new int[] {1, 2, 3, 4, 5}, 1, 3, new int[] {2, 3}),
        Arguments.of("prefix", new int[] {1, 2, 3}, 0, 2, new int[] {1, 2}),
        Arguments.of("suffix", new int[] {1, 2, 3}, 1, 3, new int[] {2, 3}),
        Arguments.of("empty-list", new int[] {}, 0, 0, new int[] {}),
        Arguments.of("empty-slice", new int[] {1, 2, 3}, 2, 2, new int[] {}),
        Arguments.of("whole", new int[] {1, 2, 3}, 0, 3, new int[] {1, 2, 3}),
        Arguments.of("single", new int[] {7}, 0, 1, new int[] {7}),
        Arguments.of("last-one", new int[] {1, 2, 3}, 2, 3, new int[] {3}),
        Arguments.of("at-end", new int[] {1, 2, 3}, 3, 3, new int[] {}),
        Arguments.of("limit", new int[] {2147483647, Integer.MIN_VALUE, 0}, 0, 2, new int[] {2147483647, Integer.MIN_VALUE}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void slice(String name, int[] items, int start, int stop, int[] want) {
    int[] items0 = items.clone();
    int[] got = SliceList.slice(items, start, stop);
    assertArrayEquals(want, got);
    if (got.length > 0) {
      got[0] ^= 1;
    }
    assertArrayEquals(items0, items);
  }

  @Test
  void badRange() {
    assertThrows(IndexOutOfBoundsException.class, () -> SliceList.slice(new int[] {1, 2, 3}, 0, 4));
    assertThrows(IndexOutOfBoundsException.class, () -> SliceList.slice(new int[] {1, 2, 3}, 2, 1));
  }
}

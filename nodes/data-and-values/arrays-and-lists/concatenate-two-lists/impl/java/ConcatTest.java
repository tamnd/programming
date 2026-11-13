// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ConcatTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("two-pairs", new int[] {1, 2}, new int[] {3, 4}, new int[] {1, 2, 3, 4}),
        Arguments.of("uneven", new int[] {5}, new int[] {6, 7, 8}, new int[] {5, 6, 7, 8}),
        Arguments.of("both-empty", new int[] {}, new int[] {}, new int[] {}),
        Arguments.of("left-empty", new int[] {}, new int[] {1, 2}, new int[] {1, 2}),
        Arguments.of("right-empty", new int[] {1, 2}, new int[] {}, new int[] {1, 2}),
        Arguments.of("duplicates", new int[] {1, 1}, new int[] {1}, new int[] {1, 1, 1}),
        Arguments.of("order", new int[] {3, 1}, new int[] {2}, new int[] {3, 1, 2}),
        Arguments.of("limit", new int[] {2147483647}, new int[] {Integer.MIN_VALUE}, new int[] {2147483647, Integer.MIN_VALUE}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void concat(String name, int[] a, int[] b, int[] want) {
    int[] a0 = a.clone();
    int[] b0 = b.clone();
    assertArrayEquals(want, Concat.concat(a, b));
    assertArrayEquals(a0, a);
    assertArrayEquals(b0, b);
  }
}

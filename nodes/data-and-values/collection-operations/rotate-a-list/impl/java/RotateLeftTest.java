// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class RotateLeftTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("by-two", new int[] {1, 2, 3, 4, 5}, 2, new int[] {3, 4, 5, 1, 2}),
        Arguments.of("by-one", new int[] {1, 2, 3}, 1, new int[] {2, 3, 1}),
        Arguments.of("zero", new int[] {1, 2, 3}, 0, new int[] {1, 2, 3}),
        Arguments.of("full-turn", new int[] {1, 2, 3}, 3, new int[] {1, 2, 3}),
        Arguments.of("more-than-length", new int[] {1, 2, 3}, 4, new int[] {2, 3, 1}),
        Arguments.of("much-larger", new int[] {1, 2, 3}, 2000000000, new int[] {3, 1, 2}),
        Arguments.of("empty", new int[] {}, 5, new int[] {}),
        Arguments.of("empty-zero", new int[] {}, 0, new int[] {}),
        Arguments.of("single", new int[] {7}, 3, new int[] {7}),
        Arguments.of("duplicates", new int[] {1, 1, 2}, 1, new int[] {1, 2, 1}),
        Arguments.of("negative-items", new int[] {-1, -2, -3}, 1, new int[] {-2, -3, -1}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void rotateLeft(String name, int[] items, int k, int[] want) {
    assertArrayEquals(want, RotateLeft.rotateLeft(items, k));
  }
}

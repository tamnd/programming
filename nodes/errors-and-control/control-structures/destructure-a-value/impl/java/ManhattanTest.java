// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ManhattanTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("first", new int[] {1, 2}, new int[] {4, 6}, 7),
        Arguments.of("on-a-line", new int[] {0, 0}, new int[] {10, 0}, 10),
        Arguments.of("down-left", new int[] {5, 5}, new int[] {2, 1}, 7),
        Arguments.of("mixed-signs", new int[] {3, -1}, new int[] {-2, 4}, 10),
        Arguments.of("same-point", new int[] {7, 7}, new int[] {7, 7}, 0),
        Arguments.of("negative", new int[] {-3, -4}, new int[] {-1, -1}, 5),
        Arguments.of("large", new int[] {1000000, -1000000}, new int[] {-1000000, 1000000}, 4000000));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void manhattan(String name, int[] p, int[] q, int want) {
    assertEquals(want, Manhattan.manhattan(p, q), name);
  }
}

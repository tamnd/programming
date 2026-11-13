// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PartitionOddTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("mixed", new int[] {1, 2, 3, 4, 5}, new int[] {1, 3, 5}, new int[] {2, 4}),
        Arguments.of("tens", new int[] {10, 15, 20, 25}, new int[] {15, 25}, new int[] {10, 20}),
        Arguments.of("empty", new int[] {}, new int[] {}, new int[] {}),
        Arguments.of("all-odd", new int[] {1, 3}, new int[] {1, 3}, new int[] {}),
        Arguments.of("all-even", new int[] {2, 4}, new int[] {}, new int[] {2, 4}),
        Arguments.of("negative", new int[] {-3, -2, -1, 0}, new int[] {-3, -1}, new int[] {-2, 0}),
        Arguments.of("zero", new int[] {0}, new int[] {}, new int[] {0}),
        Arguments.of("order", new int[] {9, 8, 1, 2}, new int[] {9, 1}, new int[] {8, 2}),
        Arguments.of("extremes", new int[] {-2147483648, 2147483647}, new int[] {2147483647}, new int[] {-2147483648}),
        Arguments.of("duplicates", new int[] {3, 3, 2, 2}, new int[] {3, 3}, new int[] {2, 2}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void partitionOdd(String name, int[] items, int[] odd, int[] even) {
    int[][] got = PartitionOdd.partitionOdd(items);
    assertEquals(2, got.length);
    assertArrayEquals(odd, got[0]);
    assertArrayEquals(even, got[1]);
  }
}

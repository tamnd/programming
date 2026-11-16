// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class MinMaxTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("mixed", List.of(3, 1, 2), 1, 3),
        Arguments.of("longer", List.of(4, 9, -2, 7, 0), -2, 9),
        Arguments.of("one", List.of(5), 5, 5),
        Arguments.of("negative", List.of(-3, -1, -7), -7, -1),
        Arguments.of("all-same", List.of(2, 2, 2), 2, 2),
        Arguments.of("int32-extremes", List.of(Integer.MAX_VALUE, 0, Integer.MIN_VALUE), Integer.MIN_VALUE, Integer.MAX_VALUE),
        Arguments.of("sorted", List.of(1, 2, 3, 4), 1, 4),
        Arguments.of("reversed", List.of(4, 3, 2, 1), 1, 4));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void minMax(String name, List<Integer> numbers, int min, int max) {
    List<Integer> before = new ArrayList<>(numbers);
    MinMax.Pair got = MinMax.minMax(numbers);
    assertEquals(min, got.min());
    assertEquals(max, got.max());
    assertEquals(before, numbers);
  }

  @Test
  void deconstructWithARecordPattern() {
    if (MinMax.minMax(List.of(3, 1, 2)) instanceof MinMax.Pair(int lo, int hi)) {
      assertEquals(1, lo);
      assertEquals(3, hi);
    }
  }
}

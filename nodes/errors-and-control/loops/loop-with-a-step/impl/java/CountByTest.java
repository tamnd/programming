// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CountByTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("by-two", 2, 8, 2, List.of(2, 4, 6, 8)),
        Arguments.of("down", 10, 0, -1, List.of(10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)),
        Arguments.of("uneven", 1, 10, 3, List.of(1, 4, 7, 10)),
        Arguments.of("past-stop", 0, 9, 4, List.of(0, 4, 8)),
        Arguments.of("down-by-three", 10, 1, -3, List.of(10, 7, 4, 1)),
        Arguments.of("empty-up", 5, 1, 1, List.of()),
        Arguments.of("empty-down", 1, 5, -1, List.of()),
        Arguments.of("single", 3, 3, 7, List.of(3)),
        Arguments.of("negative", -3, 3, 2, List.of(-3, -1, 1, 3)),
        Arguments.of("limit-up", 2147483640, 2147483647, 5, List.of(2147483640, 2147483645)),
        Arguments.of("limit-down", -2147483640, Integer.MIN_VALUE, -5, List.of(-2147483640, -2147483645)),
        Arguments.of("big-step", 0, 10, 2147483647, List.of(0)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void countBy(String name, int start, int stop, int step, List<Integer> want) {
    assertEquals(want, CountBy.countBy(start, stop, step), name);
  }
}

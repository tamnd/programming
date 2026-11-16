// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class RoundRobinTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("example", List.of(3, 1, 2), 1, List.of(1, 2, 0)),
        Arguments.of("slice-two", List.of(4, 1, 3, 2), 2, List.of(1, 3, 0, 2)),
        Arguments.of("empty", List.of(), 3, List.of()),
        Arguments.of("one", List.of(7), 2, List.of(0)),
        Arguments.of("big-slice", List.of(3, 1, 2), 10, List.of(0, 1, 2)),
        Arguments.of("slice-one", List.of(2, 2, 1), 1, List.of(2, 0, 1)),
        Arguments.of("equal-work", List.of(2, 2, 2), 1, List.of(0, 1, 2)),
        Arguments.of("long-job-first", List.of(10, 1, 1), 3, List.of(1, 2, 0)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void finishOrder(String name, List<Integer> work, int slice, List<Integer> want) {
    List<Integer> before = new ArrayList<>(work);
    assertEquals(want, RoundRobin.finishOrder(work, slice));
    assertEquals(before, work);
  }
}

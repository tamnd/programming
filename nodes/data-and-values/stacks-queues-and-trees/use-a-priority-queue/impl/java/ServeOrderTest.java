// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ServeOrderTest {
  static Stream<Arguments> cases() {
    int lo = Integer.MIN_VALUE;
    int hi = Integer.MAX_VALUE;
    return Stream.of(
        Arguments.of("example", List.of(2, 5, 1, 5), List.of(1, 3, 0, 2)),
        Arguments.of("longer", List.of(3, 1, 4, 1, 5, 9, 2, 6), List.of(5, 7, 4, 2, 0, 6, 1, 3)),
        Arguments.of("empty", List.of(), List.of()),
        Arguments.of("one", List.of(7), List.of(0)),
        Arguments.of("ties", List.of(4, 4, 4, 4), List.of(0, 1, 2, 3)),
        Arguments.of("negative", List.of(-3, -1, -2), List.of(1, 2, 0)),
        Arguments.of("int32-extremes", List.of(lo, hi, 0, lo), List.of(1, 2, 0, 3)),
        Arguments.of("sorted", List.of(1, 2, 3), List.of(2, 1, 0)),
        Arguments.of("reversed", List.of(3, 2, 1), List.of(0, 1, 2)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void serveOrder(String name, List<Integer> priorities, List<Integer> want) {
    List<Integer> before = new ArrayList<>(priorities);
    assertEquals(want, ServeOrder.serveOrder(priorities));
    assertEquals(before, priorities);
  }
}

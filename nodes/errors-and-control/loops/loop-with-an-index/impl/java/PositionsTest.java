// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PositionsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("two-matches", new String[] {"a", "b", "a", "c"}, "a", List.of(0, 2)),
        Arguments.of("words", new String[] {"red", "green", "blue", "green"}, "green", List.of(1, 3)),
        Arguments.of("empty", new String[] {}, "a", List.of()),
        Arguments.of("none", new String[] {"a", "b"}, "z", List.of()),
        Arguments.of("all", new String[] {"x", "x", "x"}, "x", List.of(0, 1, 2)),
        Arguments.of("last", new String[] {"a", "b", "c"}, "c", List.of(2)),
        Arguments.of("case", new String[] {"A", "a"}, "a", List.of(1)),
        Arguments.of("empty-string", new String[] {"", "a", ""}, "", List.of(0, 2)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void positions(String name, String[] items, String target, List<Integer> want) {
    assertEquals(want, Positions.positions(items, target), name);
  }
}

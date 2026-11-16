// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CycleTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("weekdays", "monday", 3, List.of("monday", "tuesday", "wednesday")),
        Arguments.of("wrap", "friday", 4, List.of("friday", "saturday", "sunday", "monday")),
        Arguments.of("zero-steps", "monday", 0, List.of()),
        Arguments.of("more-than-a-week", "sunday", 9, List.of("sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "monday")),
        Arguments.of("unknown", "funday", 3, List.of()),
        Arguments.of("wrong-case", "Friday", 2, List.of()),
        Arguments.of("number", "3", 2, List.of()),
        Arguments.of("empty-name", "", 1, List.of()));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void cycle(String name, String start, int steps, List<String> want) {
    assertEquals(want, Cycle.cycle(start, steps));
  }

  @Test
  void nameAndOrdinal() {
    assertEquals("FRIDAY", Cycle.Day.FRIDAY.name());
    assertEquals(4, Cycle.Day.FRIDAY.ordinal());
    assertEquals(Cycle.Day.FRIDAY, Cycle.Day.valueOf("FRIDAY"));
  }
}

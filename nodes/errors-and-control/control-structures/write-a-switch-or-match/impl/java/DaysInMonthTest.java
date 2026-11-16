// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class DaysInMonthTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("january", 1, 31),
        Arguments.of("february", 2, 28),
        Arguments.of("april", 4, 30),
        Arguments.of("july", 7, 31),
        Arguments.of("november", 11, 30),
        Arguments.of("december", 12, 31),
        Arguments.of("zero", 0, 0),
        Arguments.of("thirteen", 13, 0),
        Arguments.of("minus-one", -1, 0));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void daysInMonth(String name, int month, int want) {
    assertEquals(want, DaysInMonth.daysInMonth(month), name);
  }
}

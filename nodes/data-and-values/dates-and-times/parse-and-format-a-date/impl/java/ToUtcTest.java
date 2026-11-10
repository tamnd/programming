// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ToUtcTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("utc", "2026-11-10T12:00:00Z", "2026-11-10T12:00:00.000Z"),
        Arguments.of("positive-offset", "2026-11-10T19:30:00+07:00", "2026-11-10T12:30:00.000Z"),
        Arguments.of("negative-offset", "2026-11-10T07:00:00-05:00", "2026-11-10T12:00:00.000Z"),
        Arguments.of("back-a-day", "2026-11-11T01:00:00+07:00", "2026-11-10T18:00:00.000Z"),
        Arguments.of("forward-a-day", "2026-11-10T22:00:00-05:00", "2026-11-11T03:00:00.000Z"),
        Arguments.of("year-end", "2027-01-01T00:30:00+01:00", "2026-12-31T23:30:00.000Z"),
        Arguments.of("leap-day", "2024-02-29T23:00:00-02:00", "2024-03-01T01:00:00.000Z"),
        Arguments.of("half-hour-offset", "2026-11-10T17:30:00+05:30", "2026-11-10T12:00:00.000Z"),
        Arguments.of("quarter-hour-offset", "2026-11-10T17:45:00+05:45", "2026-11-10T12:00:00.000Z"),
        Arguments.of("unknown-offset", "2026-11-10T12:00:00-00:00", "2026-11-10T12:00:00.000Z"),
        Arguments.of("fraction-one-digit", "2026-11-10T12:00:00.5Z", "2026-11-10T12:00:00.500Z"),
        Arguments.of("fraction-three-digits", "2026-11-10T12:00:00.123+01:00", "2026-11-10T11:00:00.123Z"),
        Arguments.of("before-epoch", "1970-01-01T00:30:00+01:00", "1969-12-31T23:30:00.000Z"),
        Arguments.of("before-epoch-fraction", "1969-12-31T23:59:59.999Z", "1969-12-31T23:59:59.999Z"),
        Arguments.of("largest-offset", "2026-11-10T00:00:00+14:00", "2026-11-09T10:00:00.000Z"),
        Arguments.of("smallest-offset", "2026-11-10T00:00:00-12:00", "2026-11-10T12:00:00.000Z"),
        Arguments.of("lower-case", "2026-11-10t12:00:00z", "2026-11-10T12:00:00.000Z"),
        Arguments.of("year-one", "0001-01-01T00:00:00Z", "0001-01-01T00:00:00.000Z"),
        Arguments.of("year-9999", "9999-12-31T23:59:59.999Z", "9999-12-31T23:59:59.999Z"));
  }

  static Stream<Arguments> errors() {
    return Stream.of(
        Arguments.of("no-offset", "2026-11-10T12:00:00"),
        Arguments.of("date-only", "2026-11-10"),
        Arguments.of("space-separator", "2026-11-10 12:00:00Z"),
        Arguments.of("no-seconds", "2026-11-10T12:00Z"),
        Arguments.of("offset-no-colon", "2026-11-10T12:00:00+0700"),
        Arguments.of("offset-hours-only", "2026-11-10T12:00:00+07"),
        Arguments.of("not-a-leap-year", "2026-02-29T12:00:00Z"),
        Arguments.of("april-31", "2026-04-31T12:00:00Z"),
        Arguments.of("month-13", "2026-13-01T12:00:00Z"),
        Arguments.of("day-zero", "2026-11-00T12:00:00Z"),
        Arguments.of("hour-24", "2026-11-10T24:00:00Z"),
        Arguments.of("minute-60", "2026-11-10T12:60:00Z"),
        Arguments.of("leap-second", "2016-12-31T23:59:60Z"),
        Arguments.of("offset-hour-24", "2026-11-10T12:00:00+24:00"),
        Arguments.of("offset-minute-60", "2026-11-10T12:00:00+07:60"),
        Arguments.of("comma-fraction", "2026-11-10T12:00:00,5Z"),
        Arguments.of("trailing-space", "2026-11-10T12:00:00Z "),
        Arguments.of("empty", ""));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void convertsToUtc(String name, String timestamp, String want) {
    assertEquals(want, ToUtc.toUtc(timestamp));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("errors")
  void rejectsInvalidInput(String name, String timestamp) {
    assertThrows(IllegalArgumentException.class, () -> ToUtc.toUtc(timestamp));
  }
}

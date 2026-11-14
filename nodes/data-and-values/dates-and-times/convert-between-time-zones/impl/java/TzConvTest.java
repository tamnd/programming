// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-between-time-zones/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.time.ZonedDateTime;
import java.time.zone.ZoneRulesException;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TzConvTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("new-york-to-utc", 2024, 7, 1, 12, 0, 0, "America/New_York", "UTC", "2024-07-01T16:00:00+00:00"),
                Arguments.of("utc-to-tokyo", 2024, 1, 15, 0, 0, 0, "UTC", "Asia/Tokyo", "2024-01-15T09:00:00+09:00"),
                Arguments.of("london-to-new-york", 2024, 1, 15, 12, 0, 0, "Europe/London", "America/New_York", "2024-01-15T07:00:00-05:00"),
                Arguments.of("date-changes", 2024, 1, 15, 22, 0, 0, "America/Los_Angeles", "Asia/Tokyo", "2024-01-16T15:00:00+09:00"),
                Arguments.of("same-zone", 2024, 3, 10, 12, 0, 0, "America/New_York", "America/New_York", "2024-03-10T12:00:00-04:00"),
                Arguments.of("nonexistent-time", 2024, 3, 10, 2, 30, 0, "America/New_York", "UTC", "2024-03-10T07:30:00+00:00"),
                Arguments.of("ambiguous-time", 2024, 11, 3, 1, 30, 0, "America/New_York", "UTC", "2024-11-03T05:30:00+00:00"),
                Arguments.of("ambiguous-time-london", 2024, 10, 27, 1, 30, 0, "Europe/London", "UTC", "2024-10-27T00:30:00+00:00"),
                Arguments.of("dst-dates-differ", 2024, 3, 20, 12, 0, 0, "America/New_York", "Europe/London", "2024-03-20T16:00:00+00:00"),
                Arguments.of("southern-hemisphere", 2024, 1, 15, 12, 0, 0, "Australia/Sydney", "UTC", "2024-01-15T01:00:00+00:00"),
                Arguments.of("half-hour-offset", 2024, 1, 15, 12, 0, 0, "Asia/Kolkata", "UTC", "2024-01-15T06:30:00+00:00"),
                Arguments.of("45-minute-offset", 2024, 1, 15, 12, 0, 0, "UTC", "Asia/Kathmandu", "2024-01-15T17:45:00+05:45"),
                Arguments.of("half-hour-dst", 2024, 1, 15, 12, 0, 0, "Australia/Lord_Howe", "UTC", "2024-01-15T01:00:00+00:00"),
                Arguments.of("across-the-date-line", 2024, 1, 15, 12, 0, 0, "Pacific/Kiritimati", "Pacific/Pago_Pago", "2024-01-14T11:00:00-11:00"),
                Arguments.of("before-skipped-day", 2011, 12, 29, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-29T22:00:00+00:00"),
                Arguments.of("skipped-day", 2011, 12, 30, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-30T22:00:00+00:00"),
                Arguments.of("after-skipped-day", 2011, 12, 31, 12, 0, 0, "Pacific/Apia", "UTC", "2011-12-30T22:00:00+00:00"),
                Arguments.of("old-dst-rule", 2006, 3, 15, 12, 0, 0, "America/New_York", "UTC", "2006-03-15T17:00:00+00:00"),
                Arguments.of("posix-sign", 2024, 1, 15, 12, 0, 0, "UTC", "Etc/GMT+5", "2024-01-15T07:00:00-05:00"),
                Arguments.of("result-before-epoch", 1970, 1, 1, 0, 0, 0, "UTC", "America/New_York", "1969-12-31T19:00:00-05:00"),
                Arguments.of("far-future", 2050, 7, 1, 12, 0, 0, "America/New_York", "UTC", "2050-07-01T16:00:00+00:00"));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void convert(
            String name,
            int year,
            int month,
            int day,
            int hour,
            int minute,
            int second,
            String fromZone,
            String toZone,
            String want) {
        assertEquals(want, TzConv.convertTimeZone(year, month, day, hour, minute, second, fromZone, toZone));
    }

    @Test
    void laterOffsetAtOverlap() {
        ZonedDateTime earlier =
                ZonedDateTime.of(LocalDateTime.of(2024, 11, 3, 1, 30), ZoneId.of("America/New_York"));
        assertEquals(ZoneOffset.ofHours(-4), earlier.getOffset());
        assertEquals(ZoneOffset.ofHours(-5), earlier.withLaterOffsetAtOverlap().getOffset());
    }

    @Test
    void unknownZone() {
        assertThrows(ZoneRulesException.class, () -> TzConv.convertTimeZone(2024, 1, 1, 0, 0, 0, "Mars/Olympus", "UTC"));
    }
}

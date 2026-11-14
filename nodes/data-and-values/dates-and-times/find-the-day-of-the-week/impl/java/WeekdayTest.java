// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-day-of-the-week/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.time.DayOfWeek;
import java.time.LocalDate;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class WeekdayTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("a-sunday", 2024, 3, 10, 7),
                Arguments.of("a-monday", 2024, 3, 11, 1),
                Arguments.of("the-epoch", 1970, 1, 1, 4),
                Arguments.of("year-2000", 2000, 1, 1, 6),
                Arguments.of("a-saturday", 2024, 3, 16, 6),
                Arguments.of("leap-day", 2024, 2, 29, 4),
                Arguments.of("after-leap-day-2000", 2000, 3, 1, 3),
                Arguments.of("year-1900-is-not-leap", 1900, 3, 1, 4),
                Arguments.of("year-2100-is-not-leap", 2100, 3, 1, 1),
                Arguments.of("before-the-epoch", 1969, 12, 31, 3),
                Arguments.of("gregorian-reform", 1582, 10, 15, 5),
                Arguments.of("day-before-the-reform", 1582, 10, 4, 1),
                Arguments.of("first-day", 1, 1, 1, 1),
                Arguments.of("last-day", 9999, 12, 31, 5),
                Arguments.of("year-99", 99, 12, 31, 4),
                Arguments.of("iso-week-of-next-year", 2024, 12, 30, 1),
                Arguments.of("iso-week-of-last-year", 2027, 1, 1, 5),
                Arguments.of("month-end", 2024, 4, 30, 2)
        );
    }

    static Stream<Arguments> weeks() {
        return Stream.of(
                Arguments.of(2024, 3, 10, 2024, 10),
                Arguments.of(2024, 12, 30, 2025, 1),
                Arguments.of(2027, 1, 1, 2026, 53),
                Arguments.of(2000, 1, 1, 1999, 52),
                Arguments.of(1969, 12, 31, 1970, 1),
                Arguments.of(99, 12, 31, 99, 53)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void dayOfWeek(String name, int year, int month, int day, int want) {
        assertEquals(want, Weekday.dayOfWeek(year, month, day));
    }

    @ParameterizedTest
    @MethodSource("weeks")
    void isoWeek(int year, int month, int day, int weekYear, int week) {
        assertArrayEquals(new int[] {weekYear, week}, Weekday.isoWeek(year, month, day));
    }

    @Test
    void sundayIsSeven() {
        assertEquals(DayOfWeek.SUNDAY, LocalDate.of(2024, 3, 10).getDayOfWeek());
        assertEquals(7, DayOfWeek.SUNDAY.getValue());
    }
}

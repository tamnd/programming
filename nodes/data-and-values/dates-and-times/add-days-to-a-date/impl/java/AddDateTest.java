// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-days-to-a-date/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.time.LocalDate;
import java.time.Period;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class AddDateTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("one-day", 2024, 3, 10, 0, 0, 1, "2024-03-11"),
                Arguments.of("one-month", 2024, 1, 15, 0, 1, 0, "2024-02-15"),
                Arguments.of("one-year", 2023, 6, 1, 1, 0, 0, "2024-06-01"),
                Arguments.of("day-into-next-year", 2023, 12, 31, 0, 0, 1, "2024-01-01"),
                Arguments.of("nothing-to-add", 2024, 2, 29, 0, 0, 0, "2024-02-29"),
                Arguments.of("month-end-to-leap-february", 2024, 1, 31, 0, 1, 0, "2024-02-29"),
                Arguments.of("month-end-to-short-february", 2023, 1, 31, 0, 1, 0, "2023-02-28"),
                Arguments.of("month-end-to-30-day-month", 2024, 3, 31, 0, 1, 0, "2024-04-30"),
                Arguments.of("back-from-month-end", 2024, 5, 31, 0, -1, 0, "2024-04-30"),
                Arguments.of("back-to-leap-february", 2024, 3, 31, 0, -1, 0, "2024-02-29"),
                Arguments.of("leap-day-plus-one-year", 2024, 2, 29, 1, 0, 0, "2025-02-28"),
                Arguments.of("leap-day-plus-four-years", 2024, 2, 29, 4, 0, 0, "2028-02-29"),
                Arguments.of("year-and-month-at-once", 2024, 2, 29, 1, 1, 0, "2025-03-29"),
                Arguments.of("months-before-days", 2024, 1, 31, 0, 1, 1, "2024-03-01"),
                Arguments.of("month-into-next-year", 2023, 12, 15, 0, 1, 0, "2024-01-15"),
                Arguments.of("thirteen-months-back", 2024, 1, 15, 0, -13, 0, "2022-12-15"),
                Arguments.of("day-back-into-last-year", 2024, 1, 1, 0, 0, -1, "2023-12-31"),
                Arguments.of("year-1900-is-not-leap", 1900, 2, 28, 0, 0, 1, "1900-03-01"),
                Arguments.of("year-2100-is-not-leap", 2096, 2, 29, 4, 0, 0, "2100-02-28"),
                Arguments.of("many-days", 1970, 1, 1, 0, 0, 2932896, "9999-12-31"),
                Arguments.of("many-days-back", 9999, 12, 31, 0, 0, -3652058, "0001-01-01"),
                Arguments.of("many-months", 1, 1, 31, 0, 119987, 0, "9999-12-31")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void addToDate(String name, int year, int month, int day, int years, int months, int days, String want) {
        assertEquals(want, AddDate.addToDate(year, month, day, years, months, days));
    }

    @Test
    void plusYearsThenPlusMonthsIsNotPlusThirteenMonths() {
        LocalDate leapDay = LocalDate.of(2024, 2, 29);
        assertEquals(LocalDate.of(2025, 3, 28), leapDay.plusYears(1).plusMonths(1));
        assertEquals(LocalDate.of(2025, 3, 29), leapDay.plusMonths(13));
        assertEquals(LocalDate.of(2025, 3, 29), leapDay.plus(Period.of(1, 1, 0)));
    }
}

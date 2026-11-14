// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.time.LocalDate;
import java.time.Period;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class DateDiffTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("same-day", 2024, 3, 10, 2024, 3, 10, 0L),
                Arguments.of("next-day", 2024, 3, 10, 2024, 3, 11, 1L),
                Arguments.of("common-year", 2023, 1, 1, 2024, 1, 1, 365L),
                Arguments.of("leap-year", 2024, 1, 1, 2025, 1, 1, 366L),
                Arguments.of("end-before-start", 2024, 3, 11, 2024, 3, 10, -1L),
                Arguments.of("across-a-dst-change", 2024, 3, 9, 2024, 3, 11, 2L),
                Arguments.of("across-leap-day", 2024, 2, 28, 2024, 3, 1, 2L),
                Arguments.of("month-end-to-next-month", 2024, 1, 31, 2024, 3, 1, 30L),
                Arguments.of("year-1900-is-not-leap", 1900, 1, 1, 1901, 1, 1, 365L),
                Arguments.of("year-2000-is-leap", 2000, 1, 1, 2001, 1, 1, 366L),
                Arguments.of("400-years", 2000, 1, 1, 2400, 1, 1, 146097L),
                Arguments.of("across-the-epoch", 1969, 12, 31, 1970, 1, 1, 1L),
                Arguments.of("gregorian-reform-gap", 1582, 10, 4, 1582, 10, 15, 11L),
                Arguments.of("more-than-292-years", 1700, 1, 1, 2300, 1, 1, 219145L),
                Arguments.of("whole-range", 1, 1, 1, 9999, 12, 31, 3652058L),
                Arguments.of("whole-range-backward", 9999, 12, 31, 1, 1, 1, -3652058L),
                Arguments.of("year-99", 99, 12, 31, 100, 1, 1, 1L)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void daysBetween(String name, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, long want) {
        assertEquals(want, DateDiff.daysBetween(startYear, startMonth, startDay, endYear, endMonth, endDay));
    }

    @Test
    void periodGetDaysIsOnlyTheDayPart() {
        Period period = Period.between(LocalDate.of(2024, 1, 1), LocalDate.of(2024, 3, 1));
        assertEquals(2, period.getMonths());
        assertEquals(0, period.getDays());
    }
}

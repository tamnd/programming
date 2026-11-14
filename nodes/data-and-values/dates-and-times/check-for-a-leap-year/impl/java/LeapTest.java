// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.time.LocalDate;
import java.time.Year;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class LeapTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("divisible-by-4", 2024, true),
                Arguments.of("not-divisible-by-4", 2023, false),
                Arguments.of("century", 1900, false),
                Arguments.of("divisible-by-400", 2000, true),
                Arguments.of("next-century", 2100, false),
                Arguments.of("year-1600", 1600, true),
                Arguments.of("year-1700", 1700, false),
                Arguments.of("year-2400", 2400, true),
                Arguments.of("year-1", 1, false),
                Arguments.of("year-4", 4, true),
                Arguments.of("year-100", 100, false),
                Arguments.of("year-1500-julian-leap", 1500, false),
                Arguments.of("year-1582", 1582, false),
                Arguments.of("year-9999", 9999, false),
                Arguments.of("year-zero", 0, true),
                Arguments.of("year-minus-1", -1, false),
                Arguments.of("year-minus-4", -4, true),
                Arguments.of("year-minus-100", -100, false),
                Arguments.of("year-minus-400", -400, true)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void isLeapYear(String name, int year, boolean want) {
        assertEquals(want, Leap.isLeapYear(year));
    }

    @Test
    void sameAsArithmetic() {
        for (int year = -9999; year <= 9999; year++) {
            boolean rule = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
            assertEquals(rule, Year.isLeap(year), Integer.toString(year));
        }
    }

    @Test
    void localDateAgrees() {
        assertEquals(true, LocalDate.of(2024, 1, 1).isLeapYear());
        assertEquals(false, LocalDate.of(1900, 1, 1).isLeapYear());
    }
}

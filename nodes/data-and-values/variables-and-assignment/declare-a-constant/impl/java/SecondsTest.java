// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SecondsTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("one-day", (long) 1, (long) 86400),
                Arguments.of("one-week", (long) 7, (long) 604800),
                Arguments.of("one-year", (long) 365, (long) 31536000),
                Arguments.of("zero", (long) 0, (long) 0),
                Arguments.of("two-days", (long) 2, (long) 172800),
                Arguments.of("leap-year", (long) 366, (long) 31622400),
                Arguments.of("last-32-bit-day", (long) 24855, (long) 2147472000),
                Arguments.of("past-32-bits", (long) 24856, 2147558400L),
                Arguments.of("million-days", (long) 1000000, 86400000000L)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void secondsIn(String name, long days, long want) {
        assertEquals(want, Seconds.secondsIn(days), name);
    }

    @Test
    void secondsPerDay() {
        assertEquals(86400L, Seconds.SECONDS_PER_DAY);
    }
}

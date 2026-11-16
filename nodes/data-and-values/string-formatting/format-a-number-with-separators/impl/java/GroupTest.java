// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class GroupTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("million", (long) 1234567, "1,234,567"),
                Arguments.of("thousand", (long) 1000, "1,000"),
                Arguments.of("small", (long) 42, "42"),
                Arguments.of("zero", (long) 0, "0"),
                Arguments.of("three-digits", (long) 999, "999"),
                Arguments.of("negative", (long) -1234567, "-1,234,567"),
                Arguments.of("negative-small", (long) -999, "-999"),
                Arguments.of("negative-thousand", (long) -1000, "-1,000"),
                Arguments.of("int64-max", 9223372036854775807L, "9,223,372,036,854,775,807"),
                Arguments.of("int64-min", Long.MIN_VALUE, "-9,223,372,036,854,775,808"),
                Arguments.of("six-digits", (long) 100000, "100,000"),
                Arguments.of("seven-digits", (long) 1000000, "1,000,000")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void groupThousands(String name, long n, String want) {
        assertEquals(want, Group.groupThousands(n));
    }
}

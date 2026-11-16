// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class LimitsTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("inside", 5, 1, 10, 5),
                Arguments.of("below", -3, 0, 10, 0),
                Arguments.of("above", 42, 0, 10, 10),
                Arguments.of("at-low", 0, 0, 10, 0),
                Arguments.of("at-high", 10, 0, 10, 10),
                Arguments.of("equal-bounds", 7, 3, 3, 3),
                Arguments.of("negative-range", -50, -20, -10, -20),
                Arguments.of("zero", 0, -5, 5, 0),
                Arguments.of("int-max", 2147483647, -100, 100, 100),
                Arguments.of("int-min", -2147483648, -100, 100, -100),
                Arguments.of("full-range", -2147483648, -2147483648, 2147483647, -2147483648)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void clamp(String name, int value, int low, int high, int want) {
        assertEquals(want, Limits.clamp(value, low, high));
    }
}

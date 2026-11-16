// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FixedTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("pi-two", 3.14159, 2, "3.14"),
                Arguments.of("pad-zeros", 1.0, 3, "1.000"),
                Arguments.of("round-up", 1234.5678, 2, "1234.57"),
                Arguments.of("zero-digits", 123.456, 0, "123"),
                Arguments.of("float-error", 0.30000000000000004, 2, "0.30"),
                Arguments.of("negative", -1.234, 1, "-1.2"),
                Arguments.of("rounds-to-zero", 0.0049, 2, "0.00"),
                Arguments.of("stored-below-2-675", 2.675, 2, "2.67"),
                Arguments.of("stored-below-1-005", 1.005, 2, "1.00"),
                Arguments.of("large", 1e+20, 2, "100000000000000000000.00"),
                Arguments.of("many-digits", 0.1, 20, "0.10000000000000000555"),
                Arguments.of("integer-value", 42.0, 2, "42.00")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void fixed(String name, double x, int digits, String want) {
        assertEquals(want, Fixed.fixed(x, digits));
    }
}

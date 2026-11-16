// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.ValueSource;

class RomanTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("year-1990", 1990, "MCMXC"),
                Arguments.of("year-2008", 2008, "MMVIII"),
                Arguments.of("each-symbol-once", 1666, "MDCLXVI"),
                Arguments.of("one", 1, "I"),
                Arguments.of("four", 4, "IV"),
                Arguments.of("nine", 9, "IX"),
                Arguments.of("forty-nine", 49, "XLIX"),
                Arguments.of("ninety-nine", 99, "XCIX"),
                Arguments.of("four-hundred-forty-four", 444, "CDXLIV"),
                Arguments.of("year-1994", 1994, "MCMXCIV"),
                Arguments.of("longest", 3888, "MMMDCCCLXXXVIII"),
                Arguments.of("largest", 3999, "MMMCMXCIX")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void toRoman(String name, long n, String want) {
        assertEquals(want, Roman.toRoman(n));
        assertEquals(n, Roman.fromRoman(want));
    }

    @Test
    void roundTrip() {
        for (int n = 1; n <= 3999; n++) {
            assertEquals(n, Roman.fromRoman(Roman.toRoman(n)));
        }
    }

    @ParameterizedTest
    @ValueSource(strings = {"", "IIII", "IC", "VX", "MMMM", "iv", "ABC", "IIV", "XM"})
    void invalid(String s) {
        assertThrows(IllegalArgumentException.class, () -> Roman.fromRoman(s));
    }

    @ParameterizedTest
    @ValueSource(longs = {0, -1, 4000})
    void outOfRange(long n) {
        assertThrows(IllegalArgumentException.class, () -> Roman.toRoman(n));
    }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class DivisionTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("positive", 17, 5, 3, 2),
                Arguments.of("exact", 20, 4, 5, 0),
                Arguments.of("smaller-dividend", 3, 10, 0, 3),
                Arguments.of("negative-dividend", -17, 5, -4, 3),
                Arguments.of("negative-divisor", 17, -5, -4, -3),
                Arguments.of("both-negative", -17, -5, 3, -2),
                Arguments.of("exact-negative", -20, 4, -5, 0),
                Arguments.of("small-negative", -3, 10, -1, 7),
                Arguments.of("zero-dividend", 0, 7, 0, 0),
                Arguments.of("zero-negative-divisor", 0, -7, 0, 0),
                Arguments.of("divisor-one", 9, 1, 9, 0),
                Arguments.of("int-max", 2147483647, 2, 1073741823, 1),
                Arguments.of("int-min", -2147483648, 3, -715827883, 1),
                Arguments.of("min-by-max", -2147483648, 2147483647, -2, 2147483646),
                Arguments.of("max-by-min", 2147483647, -2147483648, -1, -1)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void divMod(String name, int a, int b, int q, int r) {
        Division.QuotientAndRemainder result = Division.divMod(a, b);
        assertEquals(q, result.quotient());
        assertEquals(r, result.remainder());
        assertEquals(new Division.QuotientAndRemainder(q, r), result);
    }
}

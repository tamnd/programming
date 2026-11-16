// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-factorial/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.math.BigInteger;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FactorialTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("five", 5, 120),
                Arguments.of("ten", 10, 3628800),
                Arguments.of("zero", 0, 1),
                Arguments.of("one", 1, 1),
                Arguments.of("largest-32-bit", 12, 479001600),
                Arguments.of("beyond-32-bit", 13, 6227020800L),
                Arguments.of("largest-64-bit", 20, 2432902008176640000L)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void factorial(String name, long n, long want) {
        assertEquals(want, Factorial.factorial(n));
        assertEquals(BigInteger.valueOf(want), Factorial.bigFactorial((int) n));
    }

    @Test
    void overflowThrows() {
        assertThrows(ArithmeticException.class, () -> Factorial.factorial(21));
        assertEquals(new BigInteger("51090942171709440000"), Factorial.bigFactorial(21));
    }
}

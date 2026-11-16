// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.math.BigInteger;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PrimeTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("small-prime", 17, true),
                Arguments.of("small-composite", 15, false),
                Arguments.of("even", 100, false),
                Arguments.of("odd-composite", 91, false),
                Arguments.of("negative", -7, false),
                Arguments.of("zero", 0, false),
                Arguments.of("one", 1, false),
                Arguments.of("two", 2, true),
                Arguments.of("three", 3, true),
                Arguments.of("square-of-prime", 25, false),
                Arguments.of("large-square", 4293001441L, false),
                Arguments.of("carmichael", 561, false),
                Arguments.of("large-prime", 1000000007, true),
                Arguments.of("mersenne-prime", 2147483647, true),
                Arguments.of("large-composite", 2147483649L, false)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void isPrime(String name, long n, boolean want) {
        assertEquals(want, Prime.isPrime(n));
        if (n >= 2) {
            assertEquals(want, BigInteger.valueOf(n).isProbablePrime(50));
        }
    }
}

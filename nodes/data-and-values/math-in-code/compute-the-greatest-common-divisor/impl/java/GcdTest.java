// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.math.BigInteger;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class GcdTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("common-factor", 12, 18, 6, 36),
                Arguments.of("coprime", 17, 5, 1, 85),
                Arguments.of("euclid-example", 48, 180, 12, 720),
                Arguments.of("zero-and-number", 0, 7, 7, 0),
                Arguments.of("both-zero", 0, 0, 0, 0),
                Arguments.of("negative", -12, 18, 6, 36),
                Arguments.of("both-negative", -4, -6, 2, 12),
                Arguments.of("equal", 9, 9, 9, 9),
                Arguments.of("one", 1, 1000000007, 1, 1000000007),
                Arguments.of("multiple", 5, 25, 5, 25),
                Arguments.of("fibonacci", 832040, 514229, 1, 427859097160L),
                Arguments.of("big-lcm", 3000000000000000000L, 6000000000000000000L, 3000000000000000000L, 6000000000000000000L),
                Arguments.of("largest", 9223372036854775807L, -9223372036854775807L, 9223372036854775807L, 9223372036854775807L)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void gcdAndLcm(String name, long a, long b, long gcd, long lcm) {
        assertArrayEquals(new long[] {gcd, lcm}, Gcd.gcdAndLcm(a, b));
        assertArrayEquals(new long[] {gcd, lcm}, Gcd.gcdAndLcm(b, a));
        assertEquals(gcd, BigInteger.valueOf(a).gcd(BigInteger.valueOf(b)).longValueExact());
    }

    @Test
    void smallestValueThrows() {
        assertThrows(ArithmeticException.class, () -> Gcd.gcdAndLcm(Long.MIN_VALUE, 0));
    }
}

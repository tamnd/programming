// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.math.BigInteger;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class FibonacciTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("ten", 10L, 55L),
                Arguments.of("twenty", 20L, 6765L),
                Arguments.of("thirty", 30L, 832040L),
                Arguments.of("zero", 0L, 0L),
                Arguments.of("one", 1L, 1L),
                Arguments.of("two", 2L, 1L),
                Arguments.of("largest-32-bit", 46L, 1836311903L),
                Arguments.of("beyond-32-bit", 47L, 2971215073L),
                Arguments.of("binet-fails", 71L, 308061521170129L),
                Arguments.of("largest-64-bit", 92L, 7540113804746346429L)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void fibonacci(String name, long n, long want) {
        assertEquals(want, Fibonacci.fibonacci(n));
        assertEquals(BigInteger.valueOf(want), Fibonacci.bigFibonacci((int) n));
    }

    @Test
    void overflowThrows() {
        assertThrows(ArithmeticException.class, () -> Fibonacci.fibonacci(93));
        assertEquals(new BigInteger("12200160415121876738"), Fibonacci.bigFibonacci(93));
    }
}

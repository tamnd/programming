// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SumProdTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("small", new long[]{1, 2, 3, 4}, 10, 24),
                Arguments.of("mixed", new long[]{3, -2, 5}, 6, -30),
                Arguments.of("empty", new long[]{}, 0, 1),
                Arguments.of("single", new long[]{7}, 7, 7),
                Arguments.of("with-zero", new long[]{5, 0, 9}, 14, 0),
                Arguments.of("two-negatives", new long[]{-2, -3}, -5, 6),
                Arguments.of("three-negatives", new long[]{-1, -2, -3}, -6, -6),
                Arguments.of("ones", new long[]{1, 1, 1}, 3, 1),
                Arguments.of("large-product", new long[]{3037000499L, 3037000499L}, 6074000998L, 9223372030926249001L),
                Arguments.of("to-the-limit", new long[]{9223372036854775806L, 1}, 9223372036854775807L, 9223372036854775806L),
                Arguments.of("beyond-double", new long[]{9007199254740993L}, 9007199254740993L, 9007199254740993L),
                Arguments.of("min", new long[]{Long.MIN_VALUE}, Long.MIN_VALUE, Long.MIN_VALUE)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void sumAndProduct(String name, long[] items, long sum, long product) {
        assertArrayEquals(new long[] {sum, product}, SumProd.sumAndProduct(items));
    }

    @Test
    void overflowThrows() {
        assertThrows(ArithmeticException.class,
                () -> SumProd.sumAndProduct(new long[] {4294967296L, 4294967296L}));
    }
}

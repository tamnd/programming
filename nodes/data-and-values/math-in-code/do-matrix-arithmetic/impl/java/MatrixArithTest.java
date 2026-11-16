// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-matrix-arithmetic/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class MatrixArithTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("two-by-two", new long[][]{new long[]{1, 2}, new long[]{3, 4}}, new long[][]{new long[]{5, 6}, new long[]{7, 8}}, new long[][]{new long[]{19, 22}, new long[]{43, 50}}),
                Arguments.of("two-by-three-times-three-by-two", new long[][]{new long[]{1, 2, 3}, new long[]{4, 5, 6}}, new long[][]{new long[]{7, 8}, new long[]{9, 10}, new long[]{11, 12}}, new long[][]{new long[]{58, 64}, new long[]{139, 154}}),
                Arguments.of("one-by-one", new long[][]{new long[]{3}}, new long[][]{new long[]{4}}, new long[][]{new long[]{12}}),
                Arguments.of("row-times-column", new long[][]{new long[]{1, 2, 3}}, new long[][]{new long[]{4}, new long[]{5}, new long[]{6}}, new long[][]{new long[]{32}}),
                Arguments.of("column-times-row", new long[][]{new long[]{1}, new long[]{2}, new long[]{3}}, new long[][]{new long[]{4, 5, 6}}, new long[][]{new long[]{4, 5, 6}, new long[]{8, 10, 12}, new long[]{12, 15, 18}}),
                Arguments.of("identity", new long[][]{new long[]{1, 0}, new long[]{0, 1}}, new long[][]{new long[]{5, 6}, new long[]{7, 8}}, new long[][]{new long[]{5, 6}, new long[]{7, 8}}),
                Arguments.of("other-order", new long[][]{new long[]{5, 6}, new long[]{7, 8}}, new long[][]{new long[]{1, 2}, new long[]{3, 4}}, new long[][]{new long[]{23, 34}, new long[]{31, 46}}),
                Arguments.of("zero-matrix", new long[][]{new long[]{0, 0}, new long[]{0, 0}}, new long[][]{new long[]{1, 2}, new long[]{3, 4}}, new long[][]{new long[]{0, 0}, new long[]{0, 0}}),
                Arguments.of("negative-numbers", new long[][]{new long[]{-1, 2}, new long[]{3, -4}}, new long[][]{new long[]{2, 0}, new long[]{1, -1}}, new long[][]{new long[]{0, -2}, new long[]{2, 4}}),
                Arguments.of("beyond-32-bit", new long[][]{new long[]{100000, 0}, new long[]{0, 100000}}, new long[][]{new long[]{100000}, new long[]{3}}, new long[][]{new long[]{10000000000L}, new long[]{300000}})
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void multiply(String name, long[][] a, long[][] b, long[][] want) {
        assertArrayEquals(want, MatrixArith.multiply(a, b));
    }

    @Test
    void transpose() {
        assertArrayEquals(new long[][] {{1, 4}, {2, 5}, {3, 6}}, MatrixArith.transpose(new long[][] {{1, 2, 3}, {4, 5, 6}}));
    }

    @Test
    void sizesMustMatch() {
        assertThrows(IllegalArgumentException.class, () -> MatrixArith.multiply(new long[][] {{1, 2}}, new long[][] {{1, 2}}));
    }
}

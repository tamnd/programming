// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/swap-two-values/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SwapTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("first-and-last", new long[]{1, 2, 3}, 0, 2, new long[]{3, 2, 1}),
                Arguments.of("middle-pair", new long[]{10, 20, 30, 40}, 1, 2, new long[]{10, 30, 20, 40}),
                Arguments.of("two-items", new long[]{5, 7}, 0, 1, new long[]{7, 5}),
                Arguments.of("single-item", new long[]{9}, 0, 0, new long[]{9}),
                Arguments.of("same-index", new long[]{1, 2, 3}, 1, 1, new long[]{1, 2, 3}),
                Arguments.of("higher-index-first", new long[]{1, 2, 3, 4}, 3, 0, new long[]{4, 2, 3, 1}),
                Arguments.of("equal-values", new long[]{4, 4}, 0, 1, new long[]{4, 4}),
                Arguments.of("negative-values", new long[]{-1, 0, 1}, 0, 2, new long[]{1, 0, -1}),
                Arguments.of("int32-extremes", new long[]{2147483647, -2147483648L}, 0, 1, new long[]{-2147483648L, 2147483647}),
                Arguments.of("eight-items", new long[]{1, 2, 3, 4, 5, 6, 7, 8}, 2, 6, new long[]{1, 2, 7, 4, 5, 6, 3, 8})
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void swapItems(String name, long[] xs, int i, int j, long[] want) {
        long[] before = xs.clone();
        assertArrayEquals(want, Swap.swapItems(xs, i, j), name);
        assertArrayEquals(before, xs, name);
    }
}

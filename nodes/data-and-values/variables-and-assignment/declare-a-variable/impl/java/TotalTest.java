// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TotalTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("one-two-three", new long[]{1, 2, 3}, (long) 6),
                Arguments.of("four-values", new long[]{10, 20, 30, 40}, (long) 100),
                Arguments.of("mixed-signs", new long[]{5, -2, 7}, (long) 10),
                Arguments.of("empty", new long[]{}, (long) 0),
                Arguments.of("single", new long[]{42}, (long) 42),
                Arguments.of("all-negative", new long[]{-1, -2, -3}, (long) -6),
                Arguments.of("zeros", new long[]{0, 0, 0}, (long) 0),
                Arguments.of("cancel-out", new long[]{7, -7}, (long) 0),
                Arguments.of("past-32-bits", new long[]{4000000000L, 5000000000L}, 9000000000L),
                Arguments.of("eight-values", new long[]{1, 1, 1, 1, 1, 1, 1, 1}, (long) 8)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void total(String name, long[] values, long want) {
        assertEquals(want, Total.total(values), name);
    }
}

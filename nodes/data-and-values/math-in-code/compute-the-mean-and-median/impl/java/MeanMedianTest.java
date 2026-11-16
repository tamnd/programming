// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.NoSuchElementException;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class MeanMedianTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("odd", new long[]{3, 1, 2}, 2.0, 2.0),
                Arguments.of("even", new long[]{4, 1, 3, 2}, 2.5, 2.5),
                Arguments.of("skewed", new long[]{1, 2, 3, 4, 100}, 22.0, 3.0),
                Arguments.of("single", new long[]{5}, 5.0, 5.0),
                Arguments.of("two-items", new long[]{1, 2}, 1.5, 1.5),
                Arguments.of("negative", new long[]{-3, -1, -2}, -2.0, -2.0),
                Arguments.of("unsorted-even", new long[]{10, 2, 8, 4}, 6.0, 6.0),
                Arguments.of("duplicates", new long[]{1, 1, 1, 5}, 2.0, 1.0),
                Arguments.of("not-an-integer", new long[]{1, 2, 2}, 1.6666666666666667, 2.0),
                Arguments.of("large", new long[]{9223372036854775807L, 9223372036854775807L}, 9223372036854775808.0, 9223372036854775808.0)
        );
    }

    static boolean close(double got, double want) {
        return Math.abs(got - want) <= Math.max(1e-9, 1e-12 * Math.abs(want));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void meanAndMedian(String name, long[] items, double mean, double median) {
        long[] before = items.clone();
        double[] got = MeanMedian.meanAndMedian(items);
        assertTrue(close(got[0], mean), "mean = " + got[0]);
        assertTrue(close(got[1], median), "median = " + got[1]);
        assertArrayEquals(before, items);
    }

    @Test
    void emptyThrows() {
        assertThrows(NoSuchElementException.class, () -> MeanMedian.meanAndMedian(new long[0]));
    }
}

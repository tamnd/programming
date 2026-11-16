// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class HigherOrderTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("one-step", 6, 1, 3),
                Arguments.of("three-steps", 6, 3, 5),
                Arguments.of("ten-steps", 27, 10, 214),
                Arguments.of("zero-times", 7, 0, 7),
                Arguments.of("start-one", 1, 1, 4),
                Arguments.of("cycle", 1, 3, 1),
                Arguments.of("past-one", 16, 10, 1),
                Arguments.of("peak-27", 27, 77, 9232),
                Arguments.of("peak-9663", 9663, 48, 27114424),
                Arguments.of("limit", 10000, 1000, 2)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void collatzAfter(String name, int start, int times, int want) {
        assertEquals(want, HigherOrder.collatzAfter(start, times));
    }

    @Test
    void repeatTakesALambda() {
        assertEquals(1024, HigherOrder.repeat(x -> x * 2, 10).apply(1));
    }

    @Test
    void repeatZeroTimesIsTheIdentity() {
        assertEquals(27, HigherOrder.repeat(HigherOrder::collatzStep, 0).apply(27));
    }
}

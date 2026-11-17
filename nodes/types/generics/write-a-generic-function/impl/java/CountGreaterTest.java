// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CountGreaterTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("some", List.of(34, 50, 25, 100, 65), 50, 2),
                Arguments.of("all", List.of(3, 4, 5), 2, 3),
                Arguments.of("none", List.of(3, 4, 5), 9, 0),
                Arguments.of("empty", List.of(), 0, 0),
                Arguments.of("equal-not-counted", List.of(5, 5, 5), 5, 0),
                Arguments.of("duplicates", List.of(7, 1, 7, 7), 6, 3),
                Arguments.of("negative", List.of(-3, -1, 0, 2), -2, 3),
                Arguments.of("min-limit", List.of(Integer.MIN_VALUE, 0, Integer.MAX_VALUE), Integer.MIN_VALUE, 2),
                Arguments.of("max-limit", List.of(Integer.MAX_VALUE, 1), Integer.MAX_VALUE, 0),
                Arguments.of("single", List.of(1), 0, 1)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void countGreater(String name, List<Integer> items, int limit, int want) {
        assertEquals(want, CountGreater.countGreater(items, limit));
    }

    @Test
    void strings() {
        assertEquals(2, CountGreater.countGreater(List.of("pear", "apple", "plum"), "orange"));
    }
}

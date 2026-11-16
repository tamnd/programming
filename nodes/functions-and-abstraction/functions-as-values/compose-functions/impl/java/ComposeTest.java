// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ComposeTest {
    static List<String> times(List<String> names, int n) {
        List<String> out = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            out.addAll(names);
        }
        return out;
    }

    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("inc-after-double", List.of("inc", "double"), 3, 7),
                Arguments.of("double-after-inc", List.of("double", "inc"), 3, 8),
                Arguments.of("three-functions", List.of("negate", "dec", "double"), 5, -9),
                Arguments.of("empty", List.of(), 42, 42),
                Arguments.of("single-zero", List.of("negate"), 0, 0),
                Arguments.of("negative", List.of("dec", "negate"), -5, 4),
                Arguments.of("low", List.of("inc"), -1000, -999),
                Arguments.of("duplicates", List.of("inc", "inc", "inc"), 7, 10),
                Arguments.of("limit-high", times(List.of("double"), 20), 1000, 1048576000),
                Arguments.of("limit-low", times(List.of("double"), 20), -1000, -1048576000),
                Arguments.of("mixed-20", times(List.of("double", "inc"), 10), 1000, 1026046),
                Arguments.of("mixed-20-swapped", times(List.of("inc", "double"), 10), 1000, 1025023)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void composeAll(String name, List<String> ops, int x, int want) {
        assertEquals(want, Compose.composeAll(ops, x));
    }

    @Test
    void composeIsGeneric() {
        List<Function<String, String>> fs = List.of(s -> s + "!", String::toUpperCase, String::strip);
        assertEquals("HELLO!", Compose.compose(fs).apply("  hello "));
    }

    @Test
    void composeOfNothingIsTheIdentity() {
        assertEquals("same", Compose.<String>compose(List.of()).apply("same"));
    }
}

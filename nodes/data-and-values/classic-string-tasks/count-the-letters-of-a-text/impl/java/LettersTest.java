// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class LettersTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("hello", "hello", new long[]{0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
                Arguments.of("pangram", "The quick brown fox jumps over the lazy dog", new long[]{1, 1, 1, 1, 3, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1}),
                Arguments.of("mississippi", "Mississippi", new long[]{0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 2, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0}),
                Arguments.of("empty", "", new long[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
                Arguments.of("no-letters", "123 !?", new long[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
                Arguments.of("mixed-case", "AaAa", new long[]{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
                Arguments.of("first-and-last", "aZ", new long[]{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}),
                Arguments.of("only-z", "zzz", new long[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3}),
                Arguments.of("white-space", " \t\n", new long[]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}),
                Arguments.of("sentence", "Count Each Letter, once.", new long[]{1, 0, 3, 0, 4, 0, 0, 1, 0, 0, 0, 1, 0, 2, 2, 0, 0, 1, 0, 3, 1, 0, 0, 0, 0, 0})
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void countLetters(String name, String text, long[] want) {
        assertArrayEquals(want, Letters.countLetters(text));
    }
}

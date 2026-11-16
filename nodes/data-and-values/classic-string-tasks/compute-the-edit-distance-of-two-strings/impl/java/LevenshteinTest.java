// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class LevenshteinTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("kitten-sitting", "kitten", "sitting", 3),
                Arguments.of("rosettacode", "rosettacode", "raisethysword", 8),
                Arguments.of("flaw-lawn", "flaw", "lawn", 2),
                Arguments.of("both-empty", "", "", 0),
                Arguments.of("empty-left", "", "abc", 3),
                Arguments.of("empty-right", "abc", "", 3),
                Arguments.of("equal", "same", "same", 0),
                Arguments.of("one-substitution", "a", "b", 1),
                Arguments.of("transposition", "ab", "ba", 2),
                Arguments.of("case", "Apple", "apple", 1),
                Arguments.of("disjoint", "abc", "xyz", 3),
                Arguments.of("prefix", "abc", "abcdef", 3),
                Arguments.of("intention", "intention", "execution", 5),
                Arguments.of("space", "a b", "ab", 1)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void levenshtein(String name, String a, String b, int want) {
        assertEquals(want, Levenshtein.levenshtein(a, b));
        assertEquals(want, Levenshtein.levenshtein(b, a));
    }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class WrapTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("fox-ten", "The quick brown fox jumps over the lazy dog", 10, List.of("The quick", "brown fox", "jumps over", "the lazy", "dog")),
                Arguments.of("fox-twenty", "The quick brown fox jumps over the lazy dog", 20, List.of("The quick brown fox", "jumps over the lazy", "dog")),
                Arguments.of("sentence", "Wrap text to a width with a greedy loop.", 15, List.of("Wrap text to a", "width with a", "greedy loop.")),
                Arguments.of("empty", "", 10, List.of()),
                Arguments.of("only-spaces", "   ", 5, List.of()),
                Arguments.of("fits", "short text", 20, List.of("short text")),
                Arguments.of("exact-fit", "abcde fghij", 5, List.of("abcde", "fghij")),
                Arguments.of("long-word", "a supercalifragilistic word", 10, List.of("a", "supercalifragilistic", "word")),
                Arguments.of("many-spaces", "one   two\tthree\nfour", 9, List.of("one two", "three", "four")),
                Arguments.of("width-one", "a b c", 1, List.of("a", "b", "c")),
                Arguments.of("line-exact", "ab cd ef", 5, List.of("ab cd", "ef"))
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void wrap(String name, String text, int width, List<String> want) {
        assertEquals(want, Wrap.wrap(text, width));
    }
}

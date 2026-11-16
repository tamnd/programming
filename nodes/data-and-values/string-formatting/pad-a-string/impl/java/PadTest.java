// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PadTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("zeros", "7", 3, "0", "007", "700"),
                Arguments.of("spaces", "abc", 6, " ", "   abc", "abc   "),
                Arguments.of("stars", "abc", 6, "*", "***abc", "abc***"),
                Arguments.of("empty-string", "", 3, "-", "---", "---"),
                Arguments.of("width-zero", "abc", 0, " ", "abc", "abc"),
                Arguments.of("exact-width", "abcd", 4, ".", "abcd", "abcd"),
                Arguments.of("shorter-width", "hello", 3, " ", "hello", "hello"),
                Arguments.of("negative-number-text", "-42", 6, "0", "000-42", "-42000"),
                Arguments.of("fill-same-as-text", "aa", 5, "a", "aaaaa", "aaaaa"),
                Arguments.of("one-char-wide", "x", 1, "_", "x", "x"),
                Arguments.of("spaces-inside", "a b", 7, ".", "....a b", "a b....")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void pad(String name, String s, int width, String fill, String left, String right) {
        assertArrayEquals(new String[] {left, right}, Pad.pad(s, width, fill));
    }
}

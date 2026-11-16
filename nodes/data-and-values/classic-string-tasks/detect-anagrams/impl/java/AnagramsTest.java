// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class AnagramsTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("listen-silent", "listen", "silent", true),
                Arguments.of("dormitory", "Dormitory", "dirty room", true),
                Arguments.of("hello-world", "hello", "world", false),
                Arguments.of("both-empty", "", "", true),
                Arguments.of("one-empty", "a", "", false),
                Arguments.of("same-set-other-counts", "aab", "abb", false),
                Arguments.of("extra-letter", "abc", "abcd", false),
                Arguments.of("same-length-other-letters", "abc", "abd", false),
                Arguments.of("case", "Listen", "SILENT", true),
                Arguments.of("same-word", "stop", "stop", true),
                Arguments.of("gentleman", "a gentleman", "elegant man", true),
                Arguments.of("they-see", "The eyes!", "They see.", true),
                Arguments.of("punctuation-only", "!?", "...", true)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void areAnagrams(String name, String a, String b, boolean want) {
        assertEquals(want, Anagrams.areAnagrams(a, b));
        assertEquals(want, Anagrams.areAnagrams(b, a));
    }
}

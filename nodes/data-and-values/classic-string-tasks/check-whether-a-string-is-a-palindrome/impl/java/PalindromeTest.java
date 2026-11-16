// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PalindromeTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("racecar", "racecar", true),
                Arguments.of("panama", "A man, a plan, a canal: Panama", true),
                Arguments.of("hello", "hello", false),
                Arguments.of("empty", "", true),
                Arguments.of("only-punctuation", "?! ...", true),
                Arguments.of("one-letter", "a", true),
                Arguments.of("two-letters", "ab", false),
                Arguments.of("mixed-case", "Aa", true),
                Arguments.of("nixon", "No 'x' in Nixon", true),
                Arguments.of("cat-i-saw", "Was it a car or a cat I saw?", true),
                Arguments.of("digits", "12321", true),
                Arguments.of("digits-not", "123", false),
                Arguments.of("zero-p", "0P", false),
                Arguments.of("race-a-car", "race a car", false),
                Arguments.of("even-near", "abca", false)
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void isPalindrome(String name, String text, boolean want) {
        assertEquals(want, Palindrome.isPalindrome(text));
    }
}

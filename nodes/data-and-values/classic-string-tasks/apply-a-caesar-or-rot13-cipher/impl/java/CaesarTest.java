// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CaesarTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("hi-2", "HI", (long) 2, "JK"),
                Arguments.of("hi-20", "HI", (long) 20, "BC"),
                Arguments.of("rot13", "Hello, World!", (long) 13, "Uryyb, Jbeyq!"),
                Arguments.of("empty", "", (long) 3, ""),
                Arguments.of("zero", "abc", (long) 0, "abc"),
                Arguments.of("full-turn", "abc", (long) 26, "abc"),
                Arguments.of("minus-one", "abc", (long) -1, "zab"),
                Arguments.of("decode", "Uryyb, Jbeyq!", (long) -13, "Hello, World!"),
                Arguments.of("above-26", "xyz", (long) 27, "yza"),
                Arguments.of("wrap", "Zz", (long) 1, "Aa"),
                Arguments.of("non-letters", "123 !? [`{@", (long) 5, "123 !? [`{@"),
                Arguments.of("non-ascii", "café ü", (long) 1, "dbgé ü"),
                Arguments.of("large", "abc", 9007199254740991L, "fgh"),
                Arguments.of("large-negative", "abc", -9007199254740991L, "vwx")
        );
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void caesar(String name, String text, long shift, String want) {
        assertEquals(want, Caesar.caesar(text, shift));
        assertEquals(text, Caesar.caesar(want, -shift));
    }
}

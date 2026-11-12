// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CharactersTest {
  // A character is a code point. U+1F642 is one character, but two UTF-16 code units and four UTF-8 bytes. A combining mark, a ZWJ and each regional indicator is a character of its own.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("ascii", "abc", List.of("a", "b", "c")),
        Arguments.of("word", "hello", List.of("h", "e", "l", "l", "o")),
        Arguments.of("space", "a b", List.of("a", " ", "b")),
        Arguments.of("empty", "", List.of()),
        Arguments.of("single", "x", List.of("x")),
        Arguments.of("non-ascii", "h\u00e9", List.of("h", "\u00e9")),
        Arguments.of("cjk", "\u65e5\u672c", List.of("\u65e5", "\u672c")),
        Arguments.of("astral", "a\ud83d\ude42b", List.of("a", "\ud83d\ude42", "b")),
        Arguments.of("combining", "e\u0301", List.of("e", "\u0301")),
        Arguments.of("regional-indicator", "\ud83c\uddfa\ud83c\uddf8", List.of("\ud83c\uddfa", "\ud83c\uddf8")),
        Arguments.of("zwj", "\ud83d\udc69\u200d\ud83d\udcbb", List.of("\ud83d\udc69", "\u200d", "\ud83d\udcbb")),
        Arguments.of("control", "a\tb\nc", List.of("a", "\t", "b", "\n", "c")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void characters(String name, String s, List<String> want) {
    assertEquals(want, Characters.characters(s));
  }
}

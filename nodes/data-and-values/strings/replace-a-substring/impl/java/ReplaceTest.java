// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ReplaceTest {
  // U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
  static Stream<Arguments> all() {
    return Stream.of(
        Arguments.of("all", "a-b-c", "-", "+", "a+b+c"),
        Arguments.of("word", "Hello, world", "world", "there", "Hello, there"),
        Arguments.of("none", "abc", "x", "y", "abc"),
        Arguments.of("empty-s", "", "a", "b", ""),
        Arguments.of("delete", "a-b-c", "-", "", "abc"),
        Arguments.of("new-contains-old", "aXa", "X", "XXX", "aXXXa"),
        Arguments.of("overlap", "aaaa", "aa", "b", "bb"),
        Arguments.of("overlap-odd", "aaa", "aa", "b", "ba"),
        Arguments.of("case-sensitive", "Hello hello", "hello", "bye", "Hello bye"),
        Arguments.of("non-ascii", "naïve caf\u00e9", "\u00e9", "e", "naïve cafe"),
        Arguments.of("astral", "a\ud83d\ude42b\ud83d\ude42", "\ud83d\ude42", "!", "a!b!"),
        Arguments.of("combining", "cafe\u0301", "e", "E", "cafE\u0301"),
        Arguments.of("precomposed", "caf\u00e9", "e", "E", "caf\u00e9"),
        Arguments.of("dot", "a.b.c", ".", "-", "a-b-c"),
        Arguments.of("dollar", "price", "price", "$&!", "$&!"),
        Arguments.of("ampersand", "a-b", "-", "&", "a&b"),
        Arguments.of("backslash", "a/b", "/", "\\", "a\\b"));
  }

  static Stream<Arguments> first() {
    return Stream.of(
        Arguments.of("first-only", "a-b-c", "-", "+", "a+b-c"),
        Arguments.of("first-none", "abc", "x", "y", "abc"),
        Arguments.of("first-overlap", "aaa", "aa", "b", "ba"),
        Arguments.of("first-astral", "\ud83d\ude42\ud83d\ude42", "\ud83d\ude42", "x", "x\ud83d\ude42"),
        Arguments.of("first-dollar", "a.b", ".", "$&", "a$&b"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("all")
  void replace(String name, String s, String old, String nw, String want) {
    assertEquals(want, Replace.replace(s, old, nw));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("first")
  void replaceFirst(String name, String s, String old, String nw, String want) {
    assertEquals(want, Replace.replaceFirst(s, old, nw));
  }
}

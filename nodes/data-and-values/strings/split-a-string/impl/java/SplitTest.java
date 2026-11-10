// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SplitTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("comma", "a,b,c", ",", 0, List.of("a", "b", "c")),
        Arguments.of("csv-line", "Ada,1815,London", ",", 0, List.of("Ada", "1815", "London")),
        Arguments.of("multi-char-separator", "a::b::c", "::", 0, List.of("a", "b", "c")),
        Arguments.of("space-separator", "one two  three", " ", 0, List.of("one", "two", "", "three")),
        Arguments.of("limit-two", "key=value=more", "=", 2, List.of("key", "value=more")),
        Arguments.of("empty", "", ",", 0, List.of("")),
        Arguments.of("no-separator", "abc", ",", 0, List.of("abc")),
        Arguments.of("leading-separator", ",a,b", ",", 0, List.of("", "a", "b")),
        Arguments.of("trailing-separators", "a,b,,", ",", 0, List.of("a", "b", "", "")),
        Arguments.of("adjacent-separators", "a,,b", ",", 0, List.of("a", "", "b")),
        Arguments.of("only-separator", ",", ",", 0, List.of("", "")),
        Arguments.of("overlapping-match", "aaa", "aa", 0, List.of("", "a")),
        Arguments.of("dot-separator", "192.168.0.1", ".", 0, List.of("192", "168", "0", "1")),
        Arguments.of("pipe-separator", "a|b|c", "|", 0, List.of("a", "b", "c")),
        Arguments.of("non-ascii", "día→naïve→日本→🙂", "→", 0, List.of("día", "naïve", "日本", "🙂")),
        Arguments.of("limit-one", "a,b,c", ",", 1, List.of("a,b,c")),
        Arguments.of("limit-above-count", "a,b", ",", 5, List.of("a", "b")),
        Arguments.of("limit-keeps-trailing-empty", "a,b,", ",", 3, List.of("a", "b", "")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void split(String name, String s, String sep, int limit, List<String> want) {
    assertEquals(want, Split.split(s, sep, limit));
  }

  static Stream<Arguments> whitespaceCases() {
    return Stream.of(
        Arguments.of("single-spaces", "a b c", List.of("a", "b", "c")),
        Arguments.of("runs-and-ends", "  lead  and trail  ", List.of("lead", "and", "trail")),
        Arguments.of("tabs-and-newlines", "a\t\tb\nc\n", List.of("a", "b", "c")),
        Arguments.of("one-word", "word", List.of("word")),
        Arguments.of("empty", "", List.of()),
        Arguments.of("only-white-space", " \t\n ", List.of()));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("whitespaceCases")
  void splitWhitespace(String name, String s, List<String> want) {
    assertEquals(want, Split.splitWhitespace(s));
  }

  static Stream<Arguments> regexCases() {
    return Stream.of(
        Arguments.of("comma-or-semicolon", "a, b;c ,d", "[ ]*[,;][ ]*", List.of("a", "b", "c", "d")),
        Arguments.of("digit-runs", "a1b22c333", "[0-9]+", List.of("a", "b", "c", "")),
        Arguments.of("adjacent-matches", "a,;b", "[,;]", List.of("a", "", "b")),
        Arguments.of("leading-and-trailing", ";a;", ";", List.of("", "a", "")),
        Arguments.of("empty", "", "[,;]", List.of("")),
        Arguments.of("no-match", "abc", "[0-9]+", List.of("abc")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("regexCases")
  void splitRegex(String name, String s, String pattern, List<String> want) {
    assertEquals(want, Split.splitRegex(s, pattern));
  }
}

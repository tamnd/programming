// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class TopWordsTest {
  record WordCount(String word, long count) {}

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("sentence", "the cat and the hat and the bat", 3, List.of(new WordCount("the", 3), new WordCount("and", 2), new WordCount("bat", 1))),
        Arguments.of("all-words", "one two two three three three", 10, List.of(new WordCount("three", 3), new WordCount("two", 2), new WordCount("one", 1))),
        Arguments.of("punctuation", "Stop. Stop, stop! Go?", 2, List.of(new WordCount("stop", 3), new WordCount("go", 1))),
        Arguments.of("lines", "red green\nblue red\ngreen red\n", 2, List.of(new WordCount("red", 3), new WordCount("green", 2))),
        Arguments.of("empty", "", 3, List.of()),
        Arguments.of("no-words", " ... ,!? \n", 3, List.of()),
        Arguments.of("case", "Word word WORD wOrD", 1, List.of(new WordCount("word", 4))),
        Arguments.of("ties", "pear fig apple fig pear apple", 3, List.of(new WordCount("apple", 2), new WordCount("fig", 2), new WordCount("pear", 2))),
        Arguments.of("fold-before-sort", "Zebra apple", 2, List.of(new WordCount("apple", 1), new WordCount("zebra", 1))),
        Arguments.of("n-zero", "a b c", 0, List.of()),
        Arguments.of("n-above-count", "a b a", 10, List.of(new WordCount("a", 2), new WordCount("b", 1))),
        Arguments.of("non-ascii", "café naïve café 日本語 naïve café", 3, List.of(new WordCount("café", 3), new WordCount("naïve", 2), new WordCount("日本語", 1))),
        Arguments.of("code-point-order", "f é e", 3, List.of(new WordCount("e", 1), new WordCount("f", 1), new WordCount("é", 1))),
        Arguments.of("apostrophe", "don't stop, don't", 3, List.of(new WordCount("don", 2), new WordCount("t", 2), new WordCount("stop", 1))),
        Arguments.of("digits", "route 66 and route 66a 66", 3, List.of(new WordCount("66", 2), new WordCount("route", 2), new WordCount("66a", 1))),
        Arguments.of("separators", "tab\there\r\nunder_score-dash", 10, List.of(new WordCount("dash", 1), new WordCount("here", 1), new WordCount("score", 1), new WordCount("tab", 1), new WordCount("under", 1))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void topWords(String name, String text, int n, List<WordCount> want) {
    List<WordCount> got =
        TopWords.topWords(text, n).stream().map(w -> new WordCount(w.word(), w.count())).toList();
    assertEquals(want, got);
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.lang.reflect.Modifier;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class WordsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three-words", "the quick fox", 3),
        Arguments.of("one-word", "hello", 1),
        Arguments.of("punctuation", "Hello, world! How are you?", 5),
        Arguments.of("empty", "", 0),
        Arguments.of("blank", " \t\n ", 0),
        Arguments.of("mixed-whitespace", "  two\twords\r\nand three  ", 4),
        Arguments.of("non-ascii", "naïve café 東京", 3));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void wordCount(String name, String text, int want) {
    assertEquals(want, Words.wordCount(text), name);
  }

  @Test
  void stringIsFinal() {
    assertTrue(Modifier.isFinal(String.class.getModifiers()));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SortByLengthTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("fruits", List.of("pear", "fig", "apple", "kiwi"), List.of("fig", "kiwi", "pear", "apple")),
        Arguments.of("three", List.of("bb", "a", "ccc"), List.of("a", "bb", "ccc")),
        Arguments.of("empty", List.of(), List.of()),
        Arguments.of("single", List.of("x"), List.of("x")),
        Arguments.of("same-length", List.of("dog", "cat", "ant"), List.of("ant", "cat", "dog")),
        Arguments.of("duplicates", List.of("b", "a", "b"), List.of("a", "b", "b")),
        Arguments.of("sorted", List.of("a", "bb", "ccc"), List.of("a", "bb", "ccc")),
        Arguments.of("reverse", List.of("ccc", "bb", "a"), List.of("a", "bb", "ccc")),
        Arguments.of("long", List.of("abcdefghij", "abcdefghi", "z"), List.of("z", "abcdefghi", "abcdefghij")),
        Arguments.of("prefix", List.of("ab", "a", "abc", "b"), List.of("a", "b", "ab", "abc")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void sortByLength(String name, List<String> words, List<String> want) {
    // List.of gives an unmodifiable list, so the function must not sort in place.
    assertEquals(want, SortByLength.sortByLength(words));
  }
}

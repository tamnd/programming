// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class GroupByLengthTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("fruits", List.of("apple", "fig", "kiwi", "pear", "plum", "date"), Map.of(3, List.of("fig"), 4, List.of("kiwi", "pear", "plum", "date"), 5, List.of("apple"))),
        Arguments.of("letters", List.of("a", "bb", "c"), Map.of(1, List.of("a", "c"), 2, List.of("bb"))),
        Arguments.of("empty", List.of(), Map.of()),
        Arguments.of("single", List.of("x"), Map.of(1, List.of("x"))),
        Arguments.of("same-length", List.of("cat", "dog", "cow"), Map.of(3, List.of("cat", "dog", "cow"))),
        Arguments.of("order", List.of("ccc", "a", "bb"), Map.of(1, List.of("a"), 2, List.of("bb"), 3, List.of("ccc"))),
        Arguments.of("duplicates", List.of("ab", "ab", "c"), Map.of(1, List.of("c"), 2, List.of("ab", "ab"))),
        Arguments.of("long", List.of("abcdefghij", "abcdefghi", "a"), Map.of(1, List.of("a"), 9, List.of("abcdefghi"), 10, List.of("abcdefghij"))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void groupByLength(String name, List<String> words, Map<Integer, List<String>> want) {
    var got = GroupByLength.groupByLength(words);
    assertEquals(new TreeMap<>(want), got);
    // The keys of the result are in ascending order.
    assertEquals(List.copyOf(new TreeMap<>(want).keySet()), List.copyOf(got.keySet()));
  }
}

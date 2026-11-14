// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class BuildMapTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("fruits", List.of(Map.entry("apple", 3), Map.entry("pear", 5), Map.entry("fig", 2)), List.of(), List.of(Map.entry("apple", 3), Map.entry("fig", 2), Map.entry("pear", 5))),
        Arguments.of("update-and-delete", List.of(Map.entry("a", 1), Map.entry("b", 2), Map.entry("a", 7), Map.entry("c", 3)), List.of("b"), List.of(Map.entry("a", 7), Map.entry("c", 3))),
        Arguments.of("empty", List.of(), List.of(), List.of()),
        Arguments.of("replace", List.of(Map.entry("k", 1), Map.entry("k", 2), Map.entry("k", 3)), List.of(), List.of(Map.entry("k", 3))),
        Arguments.of("delete-absent", List.of(Map.entry("a", 1)), List.of("z"), List.of(Map.entry("a", 1))),
        Arguments.of("delete-all", List.of(Map.entry("a", 1), Map.entry("b", 2)), List.of("a", "b"), List.of()),
        Arguments.of("delete-twice", List.of(Map.entry("a", 1), Map.entry("b", 2)), List.of("a", "a"), List.of(Map.entry("b", 2))),
        Arguments.of("zero-value", List.of(Map.entry("a", 0), Map.entry("b", -4)), List.of(), List.of(Map.entry("a", 0), Map.entry("b", -4))),
        Arguments.of("case", List.of(Map.entry("b", 1), Map.entry("B", 2), Map.entry("a", 3), Map.entry("A", 4)), List.of(), List.of(Map.entry("A", 4), Map.entry("B", 2), Map.entry("a", 3), Map.entry("b", 1))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void buildMap(String name, List<Map.Entry<String, Integer>> pairs, List<String> remove, List<Map.Entry<String, Integer>> want) {
    assertEquals(want, BuildMap.buildMap(pairs, remove));
  }
}

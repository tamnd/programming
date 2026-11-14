// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CompareSetsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("colors", List.of("red", "blue", "green"), List.of("blue", "yellow"), new CompareSets.Result(List.of("blue", "green", "red", "yellow"), List.of("blue"), List.of("green", "red"), List.of("green", "red", "yellow"), false)),
        Arguments.of("subset", List.of("b"), List.of("a", "b", "c"), new CompareSets.Result(List.of("a", "b", "c"), List.of("b"), List.of(), List.of("a", "c"), true)),
        Arguments.of("both-empty", List.of(), List.of(), new CompareSets.Result(List.of(), List.of(), List.of(), List.of(), true)),
        Arguments.of("empty-a", List.of(), List.of("x"), new CompareSets.Result(List.of("x"), List.of(), List.of(), List.of("x"), true)),
        Arguments.of("empty-b", List.of("x"), List.of(), new CompareSets.Result(List.of("x"), List.of(), List.of("x"), List.of("x"), false)),
        Arguments.of("equal", List.of("a", "b"), List.of("b", "a"), new CompareSets.Result(List.of("a", "b"), List.of("a", "b"), List.of(), List.of(), true)),
        Arguments.of("disjoint", List.of("a"), List.of("b"), new CompareSets.Result(List.of("a", "b"), List.of(), List.of("a"), List.of("a", "b"), false)),
        Arguments.of("duplicates", List.of("a", "a", "b"), List.of("b", "b"), new CompareSets.Result(List.of("a", "b"), List.of("b"), List.of("a"), List.of("a"), false)),
        Arguments.of("case", List.of("a"), List.of("A"), new CompareSets.Result(List.of("A", "a"), List.of(), List.of("a"), List.of("A", "a"), false)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void compareSets(String name, List<String> a, List<String> b, CompareSets.Result want) {
    assertEquals(want, CompareSets.compareSets(a, b));
  }
}

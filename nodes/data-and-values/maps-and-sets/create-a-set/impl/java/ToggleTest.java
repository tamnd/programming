// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ToggleTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("colors", List.of("red", "blue", "red", "green"), List.of("blue", "green")),
        Arguments.of("fresh", List.of("b", "a"), List.of("a", "b")),
        Arguments.of("empty", List.of(), List.of()),
        Arguments.of("two-times", List.of("a", "a"), List.of()),
        Arguments.of("three-times", List.of("a", "a", "a"), List.of("a")),
        Arguments.of("case", List.of("a", "A"), List.of("A", "a")),
        Arguments.of("prefix", List.of("ab", "a"), List.of("a", "ab")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void toggle(String name, List<String> words, List<String> want) {
    assertEquals(want, Toggle.toggle(words));
  }
}

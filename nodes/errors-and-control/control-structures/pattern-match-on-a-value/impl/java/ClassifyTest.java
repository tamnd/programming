// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ClassifyTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("x-axis", 5, 0, "x-axis"),
        Arguments.of("y-axis", 0, -3, "y-axis"),
        Arguments.of("diagonal", 4, 4, "diagonal"),
        Arguments.of("other", 2, 7, "other"),
        Arguments.of("origin", 0, 0, "origin"),
        Arguments.of("negative-diagonal", -2, -2, "diagonal"),
        Arguments.of("anti-diagonal", 3, -3, "other"),
        Arguments.of("negative-x-axis", -7, 0, "x-axis"),
        Arguments.of("one-one", 1, 1, "diagonal"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void classify(String name, int x, int y, String want) {
    assertEquals(want, Classify.classify(x, y), name);
  }
}

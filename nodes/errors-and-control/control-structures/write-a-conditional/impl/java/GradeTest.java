// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class GradeTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("ninety-five", 95, "A"),
        Arguments.of("eighty-five", 85, "B"),
        Arguments.of("seventy-five", 75, "C"),
        Arguments.of("sixty-five", 65, "D"),
        Arguments.of("forty", 40, "F"),
        Arguments.of("hundred", 100, "A"),
        Arguments.of("zero", 0, "F"),
        Arguments.of("ninety", 90, "A"),
        Arguments.of("eighty-nine", 89, "B"),
        Arguments.of("sixty", 60, "D"),
        Arguments.of("fifty-nine", 59, "F"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void grade(String name, int score, String want) {
    assertEquals(want, Grade.grade(score), name);
  }
}

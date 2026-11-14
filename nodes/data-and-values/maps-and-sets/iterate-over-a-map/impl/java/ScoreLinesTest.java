// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ScoreLinesTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("scores", Map.of("bob", 7, "amy", 9, "cal", 4), List.of("amy=9", "bob=7", "cal=4", "total=20")),
        Arguments.of("one", Map.of("x", 1), List.of("x=1", "total=1")),
        Arguments.of("empty", Map.of(), List.of("total=0")),
        Arguments.of("case", Map.of("b", 1, "B", 2, "a", 3, "A", 4), List.of("A=4", "B=2", "a=3", "b=1", "total=10")),
        Arguments.of("negative", Map.of("a", -5, "b", 5), List.of("a=-5", "b=5", "total=0")),
        Arguments.of("prefix", Map.of("abc", 3, "a", 2, "ab", 1), List.of("a=2", "ab=1", "abc=3", "total=6")),
        Arguments.of("reverse", Map.of("j", 1, "i", 2, "h", 3, "g", 4, "f", 5, "e", 6, "d", 7, "c", 8, "b", 9, "a", 10), List.of("a=10", "b=9", "c=8", "d=7", "e=6", "f=5", "g=4", "h=3", "i=2", "j=1", "total=55")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void scoreLines(String name, Map<String, Integer> scores, List<String> want) {
    assertEquals(want, ScoreLines.scoreLines(scores));
  }
}

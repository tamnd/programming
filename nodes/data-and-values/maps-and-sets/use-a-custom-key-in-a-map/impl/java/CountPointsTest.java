// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CountPointsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("clicks", List.of(List.of(1, 2), List.of(3, 4), List.of(1, 2)), List.of(List.of(1, 2, 2), List.of(3, 4, 1))),
        Arguments.of("swap", List.of(List.of(2, 1), List.of(1, 2)), List.of(List.of(1, 2, 1), List.of(2, 1, 1))),
        Arguments.of("empty", List.of(), List.of()),
        Arguments.of("one", List.of(List.of(0, 0)), List.of(List.of(0, 0, 1))),
        Arguments.of("negative", List.of(List.of(-1, 0), List.of(0, -1), List.of(-1, 0)), List.of(List.of(-1, 0, 2), List.of(0, -1, 1))),
        Arguments.of("joined-digits", List.of(List.of(1, 23), List.of(12, 3)), List.of(List.of(1, 23, 1), List.of(12, 3, 1))),
        Arguments.of("all-same", List.of(List.of(5, 5), List.of(5, 5), List.of(5, 5)), List.of(List.of(5, 5, 3))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void countPoints(String name, List<List<Integer>> points, List<List<Integer>> want) {
    assertEquals(want, CountPoints.countPoints(points));
  }

  @Test
  void equalPointsHaveEqualHashCodes() {
    assertEquals(new CountPoints.Point(1, 2), new CountPoints.Point(1, 2));
    assertEquals(new CountPoints.Point(1, 2).hashCode(), new CountPoints.Point(1, 2).hashCode());
  }
}

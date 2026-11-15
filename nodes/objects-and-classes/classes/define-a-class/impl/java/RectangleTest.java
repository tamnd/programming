// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class RectangleTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("three-by-four", 3, 4, 2, List.of(12, 14, 48, 28)),
        Arguments.of("square", 5, 5, 3, List.of(25, 20, 225, 60)),
        Arguments.of("zero-side", 0, 7, 2, List.of(0, 14, 0, 28)),
        Arguments.of("zero-factor", 3, 4, 0, List.of(12, 14, 0, 0)),
        Arguments.of("identity", 3, 4, 1, List.of(12, 14, 12, 14)),
        Arguments.of("limit", 1000, 1000, 46, List.of(1000000, 4000, 2116000000, 184000)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void rectangleReport(String name, int width, int height, int factor, List<Integer> want) {
    assertEquals(want, Rectangle.rectangleReport(width, height, factor));
  }

  @Test
  void instancesHaveTheirOwnFields() {
    Rectangle a = new Rectangle(1, 2);
    Rectangle b = new Rectangle(1, 2);
    a.scale(3);
    assertEquals("Rectangle(3, 6)", a.toString());
    assertEquals("Rectangle(1, 2)", b.toString());
  }
}

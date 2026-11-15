// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PointTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("right-and-up", 1, 2, 3, 4, List.of(4, 6, 7, 10)),
        Arguments.of("mixed-signs", 5, -1, -2, 3, List.of(3, 2, 5, 5)),
        Arguments.of("zero-move", 7, -3, 0, 0, List.of(7, -3, 0, 10)),
        Arguments.of("negative", -2, -5, -3, -1, List.of(-5, -6, 4, 11)),
        Arguments.of("origin", 2, 3, -2, -3, List.of(0, 0, 5, 0)),
        Arguments.of(
            "limit",
            100000000,
            -100000000,
            100000000,
            -100000000,
            List.of(200000000, -200000000, 200000000, 400000000)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void pointReport(String name, int x, int y, int dx, int dy, List<Integer> want) {
    assertEquals(want, Point.pointReport(x, y, dx, dy));
  }

  @Test
  void translateReturnsAnotherPoint() {
    Point p = new Point(1, 2);
    Point q = p.translate(3, 4);
    assertEquals(new Point(1, 2), p);
    assertEquals(new Point(4, 6), q);
    assertEquals("Point[x=4, y=6]", q.toString());
  }
}

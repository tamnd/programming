// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class Vec2Test {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("two", List.of(1L, 3L), List.of(2L, 4L), List.of(4L, 6L)),
        Arguments.of("three", List.of(1L, 2L, 3L), List.of(10L, 20L, 30L), List.of(6L, 60L)),
        Arguments.of("empty", List.of(), List.of(), List.of(0L, 0L)),
        Arguments.of("single", List.of(5L), List.of(-7L), List.of(5L, -7L)),
        Arguments.of("negative", List.of(-1L, -2L), List.of(-3L, -4L), List.of(-3L, -7L)),
        Arguments.of("cancel", List.of(3L, -3L), List.of(5L, -5L), List.of(0L, 0L)),
        Arguments.of(
            "limit",
            List.of(1_000_000_000L, 1_000_000_000L, 1_000_000_000L),
            List.of(-1_000_000_000L, -1_000_000_000L, -1_000_000_000L),
            List.of(3_000_000_000L, -3_000_000_000L)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void vectorSum(String name, List<Long> xs, List<Long> ys, List<Long> want) {
    assertEquals(want, Vec2.vectorSum(xs, ys), name);
  }

  @Test
  void methodsInPlaceOfOperators() {
    Vec2 a = new Vec2(1, 2);
    Vec2 b = new Vec2(3, 4);
    assertEquals(new Vec2(4, 6), a.plus(b));
    assertNotEquals(a, b);
    Vec2 same = new Vec2(1, 2);
    assertEquals(a, same);
    assertNotSame(a, same);
  }
}

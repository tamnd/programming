// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.OptionalLong;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class SafeDivTest {
  static final long MAX = Long.MAX_VALUE;
  static final long MIN = Long.MIN_VALUE;

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("exact", 12L, 4L, OptionalLong.of(3)),
        Arguments.of("remainder-dropped", 7L, 2L, OptionalLong.of(3)),
        Arguments.of("zero-dividend", 0L, 5L, OptionalLong.of(0)),
        Arguments.of("zero-divisor", 5L, 0L, OptionalLong.empty()),
        Arguments.of("negative-by-zero", -5L, 0L, OptionalLong.empty()),
        Arguments.of("zero-by-zero", 0L, 0L, OptionalLong.empty()),
        Arguments.of("negative-dividend", -7L, 2L, OptionalLong.of(-3)),
        Arguments.of("negative-divisor", 7L, -2L, OptionalLong.of(-3)),
        Arguments.of("both-negative", -7L, -2L, OptionalLong.of(3)),
        Arguments.of("min-by-minus-one", MIN, -1L, OptionalLong.empty()),
        Arguments.of("min-by-one", MIN, 1L, OptionalLong.of(MIN)),
        Arguments.of("max-by-minus-one", MAX, -1L, OptionalLong.of(-MAX)),
        Arguments.of("min-by-max", MIN, MAX, OptionalLong.of(-1)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void safeDiv(String name, long a, long b, OptionalLong want) {
    assertEquals(want, SafeDiv.safeDiv(a, b), name);
  }

  @Test
  void plainDivision() {
    long zero = 0;
    assertThrows(ArithmeticException.class, () -> System.out.println(5L / zero));
    long minusOne = -1;
    assertEquals(MIN, MIN / minusOne);
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-floating-point-numbers/.
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class IsCloseTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("equal", 1.5, 1.5, 1e-9, 0.0, true),
        Arguments.of("sum-of-tenths", 0.30000000000000004, 0.3, 1e-9, 0.0, true),
        Arguments.of("different", 1.0, 1.1, 1e-9, 0.0, false),
        Arguments.of("large-values", 10000000000.0, 10000000001.0, 1e-9, 0.0, true),
        Arguments.of("zero-and-zero", 0.0, 0.0, 0.0, 0.0, true),
        Arguments.of("one-ulp-apart", 1.0, 1.0000000000000002, 0.0, 0.0, false),
        Arguments.of("near-zero-relative", 1e-12, 0.0, 1e-9, 0.0, false),
        Arguments.of("near-zero-absolute", 1e-12, 0.0, 1e-9, 1e-9, true),
        Arguments.of("opposite-signs", 1e-10, -1e-10, 1e-9, 0.0, false),
        Arguments.of("opposite-signs-absolute", 1e-10, -1e-10, 1e-9, 1e-9, true),
        Arguments.of("on-the-boundary", 10.0, 9.0, 0.1, 0.0, true),
        Arguments.of("swapped-boundary", 9.0, 10.0, 0.1, 0.0, true),
        Arguments.of("past-the-boundary", 10.0, 8.875, 0.1, 0.0, false),
        Arguments.of("negative-values", -1.0, -1.0000000001, 1e-9, 0.0, true));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void check(String name, double a, double b, double relTol, double absTol, boolean want) {
    assertEquals(want, IsClose.isClose(a, b, relTol, absTol), name);
    assertEquals(want, IsClose.isClose(b, a, relTol, absTol), name);
  }

  @Test
  void nonFinite() {
    assertFalse(IsClose.isClose(Double.NaN, Double.NaN, 1e-9, 0.0));
    assertTrue(IsClose.isClose(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, 1e-9, 0.0));
    assertFalse(IsClose.isClose(Double.POSITIVE_INFINITY, Double.NEGATIVE_INFINITY, 1e-9, 0.0));
    assertFalse(IsClose.isClose(Double.POSITIVE_INFINITY, 1e308, 1e-9, 0.0));
  }

  @Test
  void equalityAndUlp() {
    assertNotEquals(0.3, 0.1 + 0.2);
    assertEquals(Math.ulp(1.0), Math.nextUp(1.0) - 1.0);
  }
}

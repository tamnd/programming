// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class RoundHalfAwayTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("down", 2.4, 2L),
        Arguments.of("up", 2.6, 3L),
        Arguments.of("negative", -2.6, -3L),
        Arguments.of("integer", 7.0, 7L),
        Arguments.of("half-even-up", 2.5, 3L),
        Arguments.of("half-odd-up", 1.5, 2L),
        Arguments.of("half-zero", 0.5, 1L),
        Arguments.of("half-negative", -2.5, -3L),
        Arguments.of("half-negative-small", -0.5, -1L),
        Arguments.of("negative-below-half", -0.4, 0L),
        Arguments.of("near-half", 0.49999999999999994, 0L),
        Arguments.of("zero", 0.0, 0L),
        Arguments.of("large-odd", 4503599627370497.0, 4503599627370497L),
        Arguments.of("large-half-negative", -1000000000000000.5, -1000000000000001L));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void check(String name, double x, long want) {
    assertEquals(want, RoundHalfAway.roundHalfAway(x), name);
  }

  @Test
  void libraryModes() {
    assertEquals(-2L, Math.round(-2.5));
    assertEquals(2.0, Math.rint(2.5));
    assertEquals(
        "2.68", new BigDecimal("2.675").setScale(2, RoundingMode.HALF_UP).toPlainString());
    assertEquals(
        "2.68", new BigDecimal("2.675").setScale(2, RoundingMode.HALF_EVEN).toPlainString());
    assertEquals("2.67", new BigDecimal(2.675).setScale(2, RoundingMode.HALF_UP).toPlainString());
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.
import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ArithTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("small", 7L, 2L, 3, new long[] {9, 5, 14, 3, 1, 343}),
        Arguments.of("exact-division", 20L, 5L, 2, new long[] {25, 15, 100, 4, 0, 400}),
        Arguments.of("negative-dividend", -7L, 2L, 3, new long[] {-5, -9, -14, -3, -1, -343}),
        Arguments.of("negative-divisor", 7L, -2L, 2, new long[] {5, 9, -14, -3, 1, 49}),
        Arguments.of("both-negative", -7L, -2L, 1, new long[] {-9, -5, 14, 3, -1, -7}),
        Arguments.of("zero-dividend", 0L, 5L, 0, new long[] {5, -5, 0, 0, 0, 1}),
        Arguments.of("zero-power", 5L, 3L, 0, new long[] {8, 2, 15, 1, 2, 1}),
        Arguments.of("divisor-larger", 3L, 7L, 1, new long[] {10, -4, 21, 0, 3, 3}),
        Arguments.of(
            "largest", 100000L, 100000L, 3,
            new long[] {200000, 0, 10000000000L, 1, 0, 1000000000000000L}),
        Arguments.of(
            "smallest", -100000L, 1L, 3,
            new long[] {-99999, -100001, -100000, -100000, 0, -1000000000000000L}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void arith(String name, long a, long b, int n, long[] want) {
    assertArrayEquals(want, Arith.arith(a, b, n), name);
  }

  @Test
  void caretIsNotAPower() {
    assertEquals(1, 2 ^ 3);
    assertEquals(8.0, Math.pow(2, 3));
  }
}

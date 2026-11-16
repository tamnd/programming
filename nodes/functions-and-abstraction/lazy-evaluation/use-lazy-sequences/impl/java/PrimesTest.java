// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PrimesTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("five", 5, new int[] {2, 3, 5, 7, 11}),
        Arguments.of("ten", 10, new int[] {2, 3, 5, 7, 11, 13, 17, 19, 23, 29}),
        Arguments.of("twenty-five", 25, new int[] {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97}),
        Arguments.of("zero", 0, new int[] {}),
        Arguments.of("one", 1, new int[] {2}),
        Arguments.of("two", 2, new int[] {2, 3}),
        Arguments.of("past-a-gap", 31, new int[] {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void firstPrimes(String name, int n, int[] want) {
    assertArrayEquals(want, Primes.firstPrimes(n));
  }

  @Test
  void primesIsLazy() {
    assertEquals(7, Primes.primes().skip(3).findFirst().getAsInt());
  }
}

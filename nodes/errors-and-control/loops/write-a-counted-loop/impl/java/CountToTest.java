// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CountToTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("ten", 10, new int[] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}),
        Arguments.of("three", 3, new int[] {1, 2, 3}),
        Arguments.of("zero", 0, new int[] {}),
        Arguments.of("one", 1, new int[] {1}),
        Arguments.of("two", 2, new int[] {1, 2}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void countTo(String name, int n, int[] want) {
    assertArrayEquals(want, CountTo.countTo(n), name);
  }
}

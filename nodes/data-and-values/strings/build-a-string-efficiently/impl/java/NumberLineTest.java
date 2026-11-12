// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class NumberLineTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("zero", 0, ""),
        Arguments.of("one", 1, "1"),
        Arguments.of("three", 3, "1,2,3"),
        Arguments.of("five", 5, "1,2,3,4,5"),
        Arguments.of("seven", 7, "1,2,3,4,5,6,7"),
        Arguments.of("nine", 9, "1,2,3,4,5,6,7,8,9"),
        Arguments.of("ten", 10, "1,2,3,4,5,6,7,8,9,10"));
  }

  // Long results: the length, the start and the end.
  static Stream<Arguments> longCases() {
    return Stream.of(
        Arguments.of("hundred", 100, 291, ",99,100"),
        Arguments.of("hundred-and-one", 101, 295, ",100,101"),
        Arguments.of("thousand", 1000, 3892, ",999,1000"),
        Arguments.of("large", 100000, 588894, ",99999,100000"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void exact(String name, int n, String want) {
    assertEquals(want, NumberLine.numberLine(n));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("longCases")
  void longResult(String name, int n, int size, String end) {
    String got = NumberLine.numberLine(n);
    assertEquals(size, got.length());
    assertTrue(got.startsWith("1,2,3,"));
    assertTrue(got.endsWith(end));
  }
}

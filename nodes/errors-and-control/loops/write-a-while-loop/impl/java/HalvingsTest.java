// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class HalvingsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("rosetta", 1024, List.of(1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1)),
        Arguments.of("ten", 10, List.of(10, 5, 2, 1)),
        Arguments.of("zero", 0, List.of()),
        Arguments.of("one", 1, List.of(1)),
        Arguments.of("odd", 7, List.of(7, 3, 1)),
        Arguments.of("limit", 2147483647, List.of(2147483647, 1073741823, 536870911, 268435455, 134217727, 67108863, 33554431, 16777215, 8388607, 4194303, 2097151, 1048575, 524287, 262143, 131071, 65535, 32767, 16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63, 31, 15, 7, 3, 1)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void halvings(String name, int n, List<Integer> want) {
    assertEquals(want, Halvings.halvings(n), name);
  }
}

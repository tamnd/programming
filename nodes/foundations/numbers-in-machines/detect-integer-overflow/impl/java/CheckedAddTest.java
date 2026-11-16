// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.OptionalLong;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class CheckedAddTest {
  static final long MAX = Long.MAX_VALUE;
  static final long MIN = Long.MIN_VALUE;

  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("small", 2L, 3L, OptionalLong.of(5)),
        Arguments.of("negative", -7L, 4L, OptionalLong.of(-3)),
        Arguments.of("beyond-32-bit", 4000000000L, 5000000000L, OptionalLong.of(9000000000L)),
        Arguments.of("max-plus-zero", MAX, 0L, OptionalLong.of(MAX)),
        Arguments.of("max-plus-one", MAX, 1L, OptionalLong.empty()),
        Arguments.of("max-plus-max", MAX, MAX, OptionalLong.empty()),
        Arguments.of("min-plus-zero", MIN, 0L, OptionalLong.of(MIN)),
        Arguments.of("min-minus-one", MIN, -1L, OptionalLong.empty()),
        Arguments.of("min-plus-min", MIN, MIN, OptionalLong.empty()),
        Arguments.of("min-plus-max", MIN, MAX, OptionalLong.of(-1)),
        Arguments.of("max-minus-one", MAX, -1L, OptionalLong.of(MAX - 1)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void checkedAdd(String name, long a, long b, OptionalLong want) {
    assertEquals(want, CheckedAdd.checkedAdd(a, b), name);
  }

  @ParameterizedTest(name = "{0} by test")
  @MethodSource("cases")
  void checkedAddByTest(String name, long a, long b, OptionalLong want) {
    assertEquals(want, CheckedAdd.checkedAddByTest(a, b), name);
  }
}

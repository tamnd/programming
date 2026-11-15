// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ZipJoinTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("rosetta", new String[] {"a", "b", "c"}, new String[] {"A", "B", "C"}, new int[] {1, 2, 3},
            List.of("aA1", "bB2", "cC3")),
        Arguments.of("words", new String[] {"x", "y"}, new String[] {"X", "Y"}, new int[] {10, 20},
            List.of("xX10", "yY20")),
        Arguments.of("shorter-first", new String[] {"a"}, new String[] {"A", "B"}, new int[] {1, 2, 3},
            List.of("aA1")),
        Arguments.of("shorter-last", new String[] {"a", "b", "c"}, new String[] {"A", "B", "C"}, new int[] {1},
            List.of("aA1")),
        Arguments.of("one-empty", new String[] {"a", "b"}, new String[] {}, new int[] {1, 2}, List.of()),
        Arguments.of("all-empty", new String[] {}, new String[] {}, new int[] {}, List.of()),
        Arguments.of("negative", new String[] {"a"}, new String[] {"A"}, new int[] {-5}, List.of("aA-5")),
        Arguments.of("empty-strings", new String[] {"", "b"}, new String[] {"A", ""}, new int[] {1, 2},
            List.of("A1", "b2")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void zipJoin(String name, String[] lower, String[] upper, int[] digits, List<String> want) {
    assertEquals(want, ZipJoin.zipJoin(lower, upper, digits), name);
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class LookupTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("prices", Map.of("apple", 3, "pear", 5), List.of("apple", "fig", "pear"), 0, List.of(3, 0, 5)),
        Arguments.of("negative-fallback", Map.of("a", 1), List.of("b", "a"), -1, List.of(-1, 1)),
        Arguments.of("empty-table", Map.of(), List.of("a", "b"), 7, List.of(7, 7)),
        Arguments.of("empty-keys", Map.of("a", 1), List.of(), 0, List.of()),
        Arguments.of("zero-value", Map.of("a", 0), List.of("a", "b"), 5, List.of(0, 5)),
        Arguments.of("repeat", Map.of("a", 1), List.of("b", "b", "a"), 2, List.of(2, 2, 1)),
        Arguments.of("case", Map.of("a", 1), List.of("A"), 0, List.of(0)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void lookup(String name, Map<String, Integer> table, List<String> keys, int fallback, List<Integer> want) {
    // A HashMap copy can change, so the test also checks that the function does not change it.
    var t = new HashMap<>(table);
    assertEquals(want, Lookup.lookup(t, keys, fallback));
    assertEquals(table, t);
  }
}

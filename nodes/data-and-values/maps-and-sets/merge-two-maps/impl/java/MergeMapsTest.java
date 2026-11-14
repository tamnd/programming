// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class MergeMapsTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("prices", Map.of("apple", 3, "pear", 5), Map.of("pear", 6, "fig", 2), List.of(Map.entry("apple", 3), Map.entry("fig", 2), Map.entry("pear", 6))),
        Arguments.of("disjoint", Map.of("a", 1), Map.of("b", 2), List.of(Map.entry("a", 1), Map.entry("b", 2))),
        Arguments.of("empty-base", Map.of(), Map.of("a", 1), List.of(Map.entry("a", 1))),
        Arguments.of("empty-override", Map.of("a", 1), Map.of(), List.of(Map.entry("a", 1))),
        Arguments.of("both-empty", Map.of(), Map.of(), List.of()),
        Arguments.of("same-keys", Map.of("a", 1, "b", 2), Map.of("a", 9, "b", 8), List.of(Map.entry("a", 9), Map.entry("b", 8))),
        Arguments.of("zero-wins", Map.of("a", 5), Map.of("a", 0), List.of(Map.entry("a", 0))),
        Arguments.of("case", Map.of("a", 1), Map.of("A", 2), List.of(Map.entry("A", 2), Map.entry("a", 1))));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void mergeMaps(String name, Map<String, Integer> base, Map<String, Integer> override, List<Map.Entry<String, Integer>> want) {
    // HashMap copies can change, so the test also checks that the function does not change them.
    var b = new HashMap<>(base);
    var o = new HashMap<>(override);
    assertEquals(want, MergeMaps.mergeMaps(b, o));
    assertEquals(base, b);
    assertEquals(override, o);
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.List;
import java.util.TreeSet;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class VersionTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("simple", List.of("1.2.0", "1.0.0", "1.1.0"), List.of("1.0.0", "1.1.0", "1.2.0")),
        Arguments.of("fields-in-order", List.of("2.0.0", "1.9.9", "1.10.0"), List.of("1.9.9", "1.10.0", "2.0.0")),
        Arguments.of("patch", List.of("0.1.10", "0.1.2", "0.1.1"), List.of("0.1.1", "0.1.2", "0.1.10")),
        Arguments.of("empty", List.of(), List.of()),
        Arguments.of("single", List.of("3.4.5"), List.of("3.4.5")),
        Arguments.of("numeric-not-text", List.of("10.0.0", "9.0.0", "1.0.0"), List.of("1.0.0", "9.0.0", "10.0.0")),
        Arguments.of("duplicates", List.of("1.0.0", "0.9.0", "1.0.0"), List.of("0.9.0", "1.0.0", "1.0.0")),
        Arguments.of("zero", List.of("0.0.1", "0.0.0"), List.of("0.0.0", "0.0.1")),
        Arguments.of(
            "limit",
            List.of("1000000.0.0", "999999.1000000.1000000"),
            List.of("999999.1000000.1000000", "1000000.0.0")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void sortVersions(String name, List<String> versions, List<String> want) {
    assertEquals(want, Version.sortVersions(versions), name);
  }

  @Test
  void orderRules() {
    Version a = new Version(1, 2, 3);
    Version b = new Version(1, 10, 0);
    assertTrue(a.compareTo(b) < 0 && b.compareTo(a) > 0);
    assertEquals(0, a.compareTo(new Version(1, 2, 3)));
    assertEquals(new Version(1, 2, 3), a);
    assertEquals(a.compareTo(new Version(1, 2, 3)) == 0, a.equals(new Version(1, 2, 3)));
    assertThrows(NullPointerException.class, () -> a.compareTo(null));
  }

  @Test
  void sortedSet() {
    TreeSet<Version> set = new TreeSet<>(List.of(new Version(2, 0, 0), new Version(1, 0, 0), new Version(1, 0, 0)));
    assertEquals(2, set.size());
    assertEquals(new Version(1, 0, 0), set.first());
  }
}

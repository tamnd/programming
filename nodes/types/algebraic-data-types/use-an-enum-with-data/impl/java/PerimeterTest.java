// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.List;
import java.util.Optional;
import java.util.OptionalInt;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class PerimeterTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("square", "square", List.of(3), OptionalInt.of(12)),
        Arguments.of("rectangle", "rectangle", List.of(2, 5), OptionalInt.of(14)),
        Arguments.of("triangle", "triangle", List.of(3, 4, 5), OptionalInt.of(12)),
        Arguments.of("unknown-kind", "circle", List.of(2), OptionalInt.empty()),
        Arguments.of("wrong-case", "Square", List.of(3), OptionalInt.empty()),
        Arguments.of("too-few-sides", "rectangle", List.of(4), OptionalInt.empty()),
        Arguments.of("too-many-sides", "square", List.of(3, 3), OptionalInt.empty()),
        Arguments.of("no-sides", "triangle", List.of(), OptionalInt.empty()),
        Arguments.of("zero-side", "square", List.of(0), OptionalInt.empty()),
        Arguments.of("negative-side", "rectangle", List.of(-2, 5), OptionalInt.empty()),
        Arguments.of("not-a-triangle", "triangle", List.of(1, 2, 3), OptionalInt.empty()),
        Arguments.of(
            "largest", "triangle", List.of(1000000, 1000000, 1000000), OptionalInt.of(3000000)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void perimeter(String name, String kind, List<Integer> sides, OptionalInt want) {
    assertEquals(want, Perimeter.perimeter(kind, sides), name);
  }

  @Test
  void variantsCarryTheirData() {
    assertEquals(
        Optional.of(new Perimeter.Rectangle(2, 5)), Perimeter.makeShape("rectangle", List.of(2, 5)));
  }
}

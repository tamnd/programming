// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.List;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ColorTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("orange", List.of(255), List.of(128), List.of(0), List.of("#FF8000")),
        Arguments.of(
            "three",
            List.of(255, 0, 0),
            List.of(0, 255, 0),
            List.of(0, 0, 255),
            List.of("#FF0000", "#00FF00", "#0000FF")),
        Arguments.of("mixed", List.of(18, 171), List.of(52, 205), List.of(86, 239), List.of("#123456", "#ABCDEF")),
        Arguments.of("empty", List.of(), List.of(), List.of(), List.of()),
        Arguments.of("single", List.of(1), List.of(2), List.of(3), List.of("#010203")),
        Arguments.of("zero", List.of(0), List.of(0), List.of(0), List.of("#000000")),
        Arguments.of("max", List.of(255), List.of(255), List.of(255), List.of("#FFFFFF")),
        Arguments.of("padding", List.of(10, 0), List.of(11, 15), List.of(12, 16), List.of("#0A0B0C", "#000F10")));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void formatColors(String name, List<Integer> reds, List<Integer> greens, List<Integer> blues, List<String> want) {
    assertEquals(want, Color.formatColors(reds, greens, blues), name);
  }

  @Test
  void forms() {
    Color c = new Color(255, 128, 0);
    assertEquals("#FF8000", c.toString());
    assertEquals("color #FF8000", "color " + c);
    assertEquals("[#FF8000]", List.of(c).toString());
  }

  @Test
  void badValue() {
    assertThrows(IllegalArgumentException.class, () -> new Color(256, 0, 0));
  }
}

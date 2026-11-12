// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class Utf8Test {
  // Each byte is a number from 0 to 255. A decode result of -1 means that the bytes are not one well-formed sequence.
  static Stream<Arguments> encodeCases() {
    return Stream.of(
        Arguments.of("ascii-a", 0x41, new int[] {65}),
        Arguments.of("euro", 0x20AC, new int[] {226, 130, 172}),
        Arguments.of("emoji", 0x1F642, new int[] {240, 159, 153, 130}),
        Arguments.of("nul", 0x0, new int[] {0}),
        Arguments.of("one-byte-max", 0x7F, new int[] {127}),
        Arguments.of("two-byte-min", 0x80, new int[] {194, 128}),
        Arguments.of("two-byte", 0xE9, new int[] {195, 169}),
        Arguments.of("two-byte-max", 0x7FF, new int[] {223, 191}),
        Arguments.of("three-byte-min", 0x800, new int[] {224, 160, 128}),
        Arguments.of("three-byte-max", 0xFFFF, new int[] {239, 191, 191}),
        Arguments.of("four-byte-min", 0x10000, new int[] {240, 144, 128, 128}),
        Arguments.of("max", 0x10FFFF, new int[] {244, 143, 191, 191}));
  }

  static Stream<Arguments> decodeCases() {
    return Stream.of(
        Arguments.of("ascii-a", new int[] {65}, 65),
        Arguments.of("e-acute", new int[] {195, 169}, 233),
        Arguments.of("emoji", new int[] {240, 159, 153, 130}, 128578),
        Arguments.of("max", new int[] {244, 143, 191, 191}, 1114111),
        Arguments.of("replacement", new int[] {239, 191, 189}, 65533),
        Arguments.of("empty", new int[] {}, -1),
        Arguments.of("stray-continuation", new int[] {128}, -1),
        Arguments.of("truncated", new int[] {195}, -1),
        Arguments.of("bad-continuation", new int[] {195, 40}, -1),
        Arguments.of("overlong-two", new int[] {192, 129}, -1),
        Arguments.of("overlong-three", new int[] {224, 128, 128}, -1),
        Arguments.of("surrogate", new int[] {237, 160, 128}, -1),
        Arguments.of("above-max", new int[] {244, 144, 128, 128}, -1),
        Arguments.of("two-sequences", new int[] {65, 66}, -1));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("encodeCases")
  void encodeUtf8(String name, int cp, int[] want) {
    assertArrayEquals(want, Utf8.encodeUtf8(cp));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("decodeCases")
  void decodeUtf8(String name, int[] b, int want) {
    assertEquals(want, Utf8.decodeUtf8(b));
  }
}

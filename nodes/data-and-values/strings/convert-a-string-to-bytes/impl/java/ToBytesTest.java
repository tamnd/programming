// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ToBytesTest {
  // The encoding is UTF-8. U+00E9 is 2 bytes, U+20AC is 3 bytes and U+1F642 is 4 bytes. In Latin-1, U+00FF would be 1 byte. A Java byte is signed, so the test compares b & 0xff.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("ascii", "abc", new int[] {97, 98, 99}),
        Arguments.of("letter", "A", new int[] {65}),
        Arguments.of("punct", "hi!", new int[] {104, 105, 33}),
        Arguments.of("empty", "", new int[] {}),
        Arguments.of("control", "\t\n", new int[] {9, 10}),
        Arguments.of("two-byte", "\u00e9", new int[] {195, 169}),
        Arguments.of("latin-1-max", "\u00ff", new int[] {195, 191}),
        Arguments.of("three-byte", "\u20ac", new int[] {226, 130, 172}),
        Arguments.of("bom", "\ufeff", new int[] {239, 187, 191}),
        Arguments.of("astral", "\ud83d\ude42", new int[] {240, 159, 153, 130}),
        Arguments.of("combining", "e\u0301", new int[] {101, 204, 129}),
        Arguments.of("mixed", "a\u00e9\ud83d\ude42", new int[] {97, 195, 169, 240, 159, 153, 130}));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void toBytes(String name, String s, int[] want) {
    byte[] got = ToBytes.toBytes(s);
    int[] values = new int[got.length];
    for (int i = 0; i < got.length; i++) {
      values[i] = got[i] & 0xff;
    }
    assertArrayEquals(want, values);
  }
}

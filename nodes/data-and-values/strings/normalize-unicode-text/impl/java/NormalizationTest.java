// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class NormalizationTest {
  // U+00E9 is NFC, and e U+0301 is NFD. Both are the same text.
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("nfc-composes", "e\u0301", "NFC", "\u00e9"),
        Arguments.of("nfd-decomposes", "\u00e9", "NFD", "e\u0301"),
        Arguments.of("ascii", "abc", "NFC", "abc"),
        Arguments.of("empty", "", "NFD", ""),
        Arguments.of("already-nfc", "\u00e9", "NFC", "\u00e9"),
        Arguments.of("already-nfd", "e\u0301", "NFD", "e\u0301"),
        Arguments.of("mixed-nfc", "cafe\u0301 caf\u00e9", "NFC", "caf\u00e9 caf\u00e9"),
        Arguments.of("mixed-nfd", "cafe\u0301 caf\u00e9", "NFD", "cafe\u0301 cafe\u0301"),
        Arguments.of("upper-grave", "E\u0300", "NFC", "\u00c8"),
        Arguments.of("diaeresis", "u\u0308ber", "NFC", "\u00fcber"),
        Arguments.of("circumflex", "\u00ee", "NFD", "i\u0302"),
        Arguments.of("umlauts", "\u00c4\u00d6\u00dc\u00e4\u00f6\u00fc", "NFD", "A\u0308O\u0308U\u0308a\u0308o\u0308u\u0308"),
        Arguments.of("spacing-accent", "\u00b4e", "NFC", "\u00b4e"),
        Arguments.of("no-break-space", "a\u00a0b", "NFD", "a\u00a0b"),
        Arguments.of("sharp-s", "stra\u00dfe", "NFD", "stra\u00dfe"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void normalize(String name, String s, String form, String want) {
    assertEquals(want, Normalization.normalize(s, form));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.HexFormat;
import java.util.Locale;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class Base64CodecTest {
  private static final HexFormat HEX = HexFormat.of();

  private static Base64Codec.Alphabet alphabet(String name) {
    return Base64Codec.Alphabet.valueOf(name.toUpperCase(Locale.ROOT));
  }

  // The bytes are written in hexadecimal.
  static Stream<Arguments> encodeCases() {
    return Stream.of(
        Arguments.of("rfc-f", "66", "standard", "Zg=="),
        Arguments.of("rfc-fo", "666f", "standard", "Zm8="),
        Arguments.of("rfc-foo", "666f6f", "standard", "Zm9v"),
        Arguments.of("rfc-foob", "666f6f62", "standard", "Zm9vYg=="),
        Arguments.of("rfc-fooba", "666f6f6261", "standard", "Zm9vYmE="),
        Arguments.of("rfc-foobar", "666f6f626172", "standard", "Zm9vYmFy"),
        Arguments.of("hello-standard", "48656c6c6f2c20776f726c6421", "standard", "SGVsbG8sIHdvcmxkIQ=="),
        Arguments.of("hello-url", "48656c6c6f2c20776f726c6421", "url", "SGVsbG8sIHdvcmxkIQ"),
        Arguments.of("empty-standard", "", "standard", ""),
        Arguments.of("empty-url", "", "url", ""),
        Arguments.of("two-pad-chars", "00", "standard", "AA=="),
        Arguments.of("one-pad-char", "0000", "standard", "AAA="),
        Arguments.of("zero-bytes", "000000", "standard", "AAAA"),
        Arguments.of("plus-and-slash", "fbff", "standard", "+/8="),
        Arguments.of("minus-and-underscore", "fbff", "url", "-_8"),
        Arguments.of("all-62-standard", "fbefbe", "standard", "++++"),
        Arguments.of("all-62-url", "fbefbe", "url", "----"),
        Arguments.of("all-63-standard", "ffffff", "standard", "////"),
        Arguments.of("all-63-url", "ffffff", "url", "____"),
        Arguments.of("url-one-byte", "66", "url", "Zg"),
        Arguments.of("url-two-bytes", "666f", "url", "Zm8"),
        Arguments.of("non-ascii-standard", "c3a9e282acf09f9982", "standard", "w6nigqzwn5mC"),
        Arguments.of("non-ascii-url", "c3a9e282acf09f9982", "url", "w6nigqzwn5mC"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("encodeCases")
  void encode(String name, String data, String alphabet, String want) {
    assertEquals(want, Base64Codec.encode(HEX.parseHex(data), alphabet(alphabet)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("encodeCases")
  void roundTrip(String name, String data, String alphabet, String want) {
    String text = Base64Codec.encode(HEX.parseHex(data), alphabet(alphabet));
    assertEquals(data, HEX.formatHex(Base64Codec.decode(text, alphabet(alphabet))));
  }

  static Stream<Arguments> decodeCases() {
    return Stream.of(
        Arguments.of("rfc-empty", "", "standard", ""),
        Arguments.of("rfc-f", "Zg==", "standard", "66"),
        Arguments.of("rfc-fo", "Zm8=", "standard", "666f"),
        Arguments.of("rfc-foo", "Zm9v", "standard", "666f6f"),
        Arguments.of("rfc-foob", "Zm9vYg==", "standard", "666f6f62"),
        Arguments.of("rfc-fooba", "Zm9vYmE=", "standard", "666f6f6261"),
        Arguments.of("rfc-foobar", "Zm9vYmFy", "standard", "666f6f626172"),
        Arguments.of("empty-url", "", "url", ""),
        Arguments.of("url-foob", "Zm9vYg", "url", "666f6f62"),
        Arguments.of("url-fooba", "Zm9vYmE", "url", "666f6f6261"),
        Arguments.of("url-foobar", "Zm9vYmFy", "url", "666f6f626172"),
        Arguments.of("plus-and-slash", "+/8=", "standard", "fbff"),
        Arguments.of("minus-and-underscore", "-_8", "url", "fbff"),
        Arguments.of("all-63-url", "____", "url", "ffffff"),
        Arguments.of("zero-bytes", "AAAA", "standard", "000000"),
        Arguments.of("non-ascii-standard", "w6nigqzwn5mC", "standard", "c3a9e282acf09f9982"),
        Arguments.of("non-ascii-url", "w6nigqzwn5mC", "url", "c3a9e282acf09f9982"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("decodeCases")
  void decode(String name, String text, String alphabet, String want) {
    assertEquals(want, HEX.formatHex(Base64Codec.decode(text, alphabet(alphabet))));
  }

  static Stream<Arguments> errorCases() {
    return Stream.of(
        Arguments.of("plus-in-url", "+/8", "url"),
        Arguments.of("minus-in-standard", "-_8=", "standard"),
        Arguments.of("bang", "Zm9v!", "standard"),
        Arguments.of("non-ascii-char", "Zm9vé===", "standard"),
        Arguments.of("space", "Zm9v YmFy", "standard"),
        Arguments.of("line-feed", "Zm9v\nYmFy", "standard"),
        Arguments.of("trailing-crlf", "Zm9vYmFy\r\n", "standard"),
        Arguments.of("line-feed-url", "Zm9v\nYmFy", "url"),
        Arguments.of("missing-padding", "Zm8", "standard"),
        Arguments.of("missing-one-pad", "Zg=", "standard"),
        Arguments.of("excess-padding", "Zm9v=", "standard"),
        Arguments.of("padding-in-middle", "Zm=9", "standard"),
        Arguments.of("three-pads", "Z===", "standard"),
        Arguments.of("only-padding", "====", "standard"),
        Arguments.of("padding-then-data", "Zg==Zm8=", "standard"),
        Arguments.of("padding-in-url", "Zm8=", "url"),
        Arguments.of("length-1-mod-4-standard", "Zm9vY", "standard"),
        Arguments.of("length-1-mod-4-url", "Zm9vY", "url"),
        Arguments.of("one-char-url", "Z", "url"),
        Arguments.of("pad-bits-one-byte", "QR==", "standard"),
        Arguments.of("pad-bits-two-bytes", "Zm9=", "standard"),
        Arguments.of("pad-bits-url", "Zm9", "url"));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("errorCases")
  void invalid(String name, String text, String alphabet) {
    assertThrows(
        IllegalArgumentException.class, () -> Base64Codec.decode(text, alphabet(alphabet)));
  }
}

// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class ParseIntegerTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("decimal", "42", 10, 42),
        Arguments.of("negative", "-17", 10, -17),
        Arguments.of("hex", "ff", 16, 255),
        Arguments.of("binary", "101010", 2, 42),
        Arguments.of("base-36", "zz", 36, 1295),
        Arguments.of("plus-sign", "+5", 10, 5),
        Arguments.of("negative-zero", "-0", 10, 0),
        Arguments.of("zero", "0", 10, 0),
        Arguments.of("leading-zeros", "007", 10, 7),
        Arguments.of("many-leading-zeros", "0".repeat(5000) + "42", 10, 42),
        Arguments.of("max", "9223372036854775807", 10, 9223372036854775807L),
        Arguments.of("min", "-9223372036854775808", 10, Long.MIN_VALUE),
        Arguments.of("max-hex", "7fffffffffffffff", 16, 9223372036854775807L),
        Arguments.of("min-binary", "-1000000000000000000000000000000000000000000000000000000000000000", 2, Long.MIN_VALUE),
        Arguments.of("upper-case", "FF", 16, 255),
        Arguments.of("mixed-case", "Zz", 36, 1295),
        Arguments.of("above-safe-integer", "9007199254740993", 10, 9007199254740993L));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void parses(String name, String s, int radix, long want) {
    assertEquals(want, ParseInteger.parseInteger(s, radix));
  }

  static Stream<Arguments> invalid() {
    return Stream.of(
        Arguments.of("empty", "", 10),
        Arguments.of("sign-only", "-", 10),
        Arguments.of("plus-only", "+", 10),
        Arguments.of("double-sign", "+-5", 10),
        Arguments.of("leading-space", " 42", 10),
        Arguments.of("trailing-space", "42 ", 10),
        Arguments.of("trailing-newline", "42\n", 10),
        Arguments.of("trailing-garbage", "42px", 10),
        Arguments.of("underscore", "1_000", 10),
        Arguments.of("thousands-separator", "1,000", 10),
        Arguments.of("decimal-point", "4.0", 10),
        Arguments.of("exponent", "1e3", 10),
        Arguments.of("hex-prefix", "0x1A", 16),
        Arguments.of("binary-prefix", "0b101", 2),
        Arguments.of("digit-outside-base", "102", 2),
        Arguments.of("letter-outside-base", "fg", 16),
        Arguments.of("arabic-indic-digits", "\u0661\u0662", 10),
        Arguments.of("fullwidth-digits", "\uff11\uff12", 10),
        Arguments.of("kelvin-sign", "\u212a", 36));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("invalid")
  void rejectsInvalid(String name, String s, int radix) {
    assertThrows(NumberFormatException.class, () -> ParseInteger.parseInteger(s, radix));
  }

  static Stream<Arguments> outOfRange() {
    return Stream.of(
        Arguments.of("max-plus-one", "9223372036854775808", 10),
        Arguments.of("min-minus-one", "-9223372036854775809", 10),
        Arguments.of("two-to-the-64", "18446744073709551616", 10),
        Arguments.of("hex-sign-bit", "8000000000000000", 16),
        Arguments.of("hex-all-ones", "ffffffffffffffff", 16),
        Arguments.of("min-times-ten", "-92233720368547758080", 10),
        Arguments.of("many-digits", "9".repeat(5000), 10));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("outOfRange")
  void rejectsOutOfRange(String name, String s, int radix) {
    assertThrows(ArithmeticException.class, () -> ParseInteger.parseInteger(s, radix));
  }
}

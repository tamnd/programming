// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-big-integers/.
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.math.BigInteger;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class BigPowerTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("small", 2, 10, "1024"),
        Arguments.of("two-to-the-100", 2, 100, "1267650600228229401496703205376"),
        Arguments.of("ten-to-the-30", 10, 30, "1" + "0".repeat(30)),
        Arguments.of("zero-exponent", 7, 0, "1"),
        Arguments.of("zero-to-the-zero", 0, 0, "1"),
        Arguments.of("zero-base", 0, 5, "0"),
        Arguments.of("minus-one-odd", -1, 199, "-1"),
        Arguments.of("negative-odd", -3, 41, "-36472996377170786403"),
        Arguments.of("negative-even", -3, 40, "12157665459056928801"),
        Arguments.of("int64-max-plus-one", 2, 63, "9223372036854775808"),
        Arguments.of("uint64-max-plus-one", 2, 64, "18446744073709551616"),
        Arguments.of(
            "two-to-the-200", 2, 200, "1606938044258990275541962092341162602522202993782792835301376"),
        Arguments.of("largest", 1000, 200, "1" + "0".repeat(600)),
        Arguments.of("smallest", -1000, 199, "-1" + "0".repeat(597)));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void check(String name, int base, int exp, String want) {
    assertEquals(want, BigPower.bigPower(base, exp), name);
  }

  @Test
  void fixedWidth() {
    BigInteger big = BigInteger.TWO.pow(63);
    assertEquals(big, new BigInteger("9223372036854775808"));
    assertEquals(Long.MAX_VALUE, big.subtract(BigInteger.ONE).longValueExact());
    assertThrows(ArithmeticException.class, big::longValueExact);
    assertEquals(Long.MIN_VALUE, big.longValue());
  }
}

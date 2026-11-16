// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.List;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class MoneyTest {
  static Stream<Arguments> cases() {
    return Stream.of(
        Arguments.of("two-equal", List.of(1250L, 1250L), List.of("EUR", "EUR"), 1),
        Arguments.of("different-amounts", List.of(100L, 200L, 100L), List.of("USD", "USD", "USD"), 2),
        Arguments.of("mixed", List.of(500L, 500L, 500L, 700L), List.of("EUR", "USD", "EUR", "EUR"), 3),
        Arguments.of("empty", List.of(), List.of(), 0),
        Arguments.of("case", List.of(999L, 999L, 999L), List.of("gbp", "GBP", "Gbp"), 1),
        Arguments.of("other-currency", List.of(0L, 0L, 0L), List.of("EUR", "USD", "JPY"), 3),
        Arguments.of("negative", List.of(-5L, 5L, -5L), List.of("EUR", "EUR", "EUR"), 2),
        Arguments.of(
            "limit",
            List.of(1_000_000_000_000L, -1_000_000_000_000L, 1_000_000_000_000L),
            List.of("usd", "USD", "USD"),
            2));
  }

  @ParameterizedTest(name = "{0}")
  @MethodSource("cases")
  void countDistinct(String name, List<Long> amounts, List<String> currencies, int want) {
    assertEquals(want, Money.countDistinct(amounts, currencies), name);
  }

  @Test
  void equalityRules() {
    Money a = new Money(1250, "EUR");
    Money b = new Money(1250, "eur");
    assertTrue(a.equals(a));
    assertTrue(a.equals(b) && b.equals(a));
    assertEquals(a.hashCode(), b.hashCode());
    assertNotEquals(a, new Money(1251, "EUR"));
    assertNotEquals(a, new Money(1250, "USD"));
    assertFalse(a.equals(null));
    assertFalse(a.equals("1250 EUR"));
    assertFalse(a == b);
  }

  @Test
  void mapKey() {
    Map<Money, String> names = Map.of(new Money(500, "USD"), "lunch");
    assertEquals("lunch", names.get(new Money(500, "usd")));
  }
}

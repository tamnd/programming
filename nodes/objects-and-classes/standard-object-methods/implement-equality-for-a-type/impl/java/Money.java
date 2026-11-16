// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import java.util.Set;

/** An amount in cents and a currency code. The class is final, so no subclass can break equals. */
public final class Money {
  private final long cents;
  private final String currency;

  public Money(long cents, String currency) {
    this.cents = cents;
    this.currency = currency.toUpperCase(Locale.ROOT); // "eur" and "EUR" are the same currency
  }

  public long cents() {
    return cents;
  }

  public String currency() {
    return currency;
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) {
      return true;
    }
    if (!(o instanceof Money other)) {
      return false; // also for null
    }
    return cents == other.cents && currency.equals(other.currency);
  }

  @Override
  public int hashCode() {
    return Objects.hash(cents, currency); // the same fields as equals
  }

  @Override
  public String toString() {
    return "Money[" + cents + " " + currency + "]";
  }

  public static int countDistinct(List<Long> amounts, List<String> currencies) {
    Set<Money> seen = new HashSet<>();
    for (int i = 0; i < amounts.size(); i++) {
      seen.add(new Money(amounts.get(i), currencies.get(i)));
    }
    return seen.size();
  }
}

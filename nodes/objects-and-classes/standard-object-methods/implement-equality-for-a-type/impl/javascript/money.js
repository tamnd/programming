// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

export class Money {
  #cents;
  #currency;

  constructor(cents, currency) {
    this.#cents = cents;
    this.#currency = currency.toUpperCase(); // "eur" and "EUR" are the same currency
    Object.freeze(this);
  }

  get cents() {
    return this.#cents;
  }

  get currency() {
    return this.#currency;
  }

  equals(other) {
    return other instanceof Money && this.#cents === other.#cents && this.#currency === other.#currency;
  }

  // Set, Map and === compare objects by identity. Equal values have equal keys, so code
  // that needs value equality in a Set or a Map stores the key.
  key() {
    return `${this.#currency} ${this.#cents}`;
  }

  toString() {
    return `Money(${this.#cents}, ${this.#currency})`;
  }
}

export function countDistinct(amounts, currencies) {
  const seen = new Set();
  amounts.forEach((amount, i) => seen.add(new Money(amount, currencies[i]).key()));
  return seen.size;
}

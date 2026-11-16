// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

export class Money {
  readonly #cents: number;
  readonly #currency: string;

  constructor(cents: number, currency: string) {
    this.#cents = cents;
    this.#currency = currency.toUpperCase(); // "eur" and "EUR" are the same currency
    Object.freeze(this);
  }

  get cents(): number {
    return this.#cents;
  }

  get currency(): string {
    return this.#currency;
  }

  equals(other: unknown): boolean {
    return other instanceof Money && this.#cents === other.#cents && this.#currency === other.#currency;
  }

  // Set, Map and === compare objects by identity, so value equality in a Set or a Map uses this key.
  key(): string {
    return `${this.#currency} ${this.#cents}`;
  }

  toString(): string {
    return `Money(${this.#cents}, ${this.#currency})`;
  }
}

export function countDistinct(amounts: number[], currencies: string[]): number {
  const seen = new Set<string>();
  amounts.forEach((amount, i) => seen.add(new Money(amount, currencies[i]).key()));
  return seen.size;
}

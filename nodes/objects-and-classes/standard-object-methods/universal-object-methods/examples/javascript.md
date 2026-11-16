```javascript run id=equals-by-convention
class Money {
  constructor(cents, currency) {
    this.cents = cents;
    this.currency = currency;
  }

  equals(other) {
    return other instanceof Money && this.cents === other.cents && this.currency === other.currency;
  }

  key() {
    return `${this.currency}:${this.cents}`; // a primitive that a Set or Map can compare
  }

  toString() {
    return `${(this.cents / 100).toFixed(2)} ${this.currency}`;
  }
}

const a = new Money(1250, "EUR");
const b = new Money(1250, "EUR");
console.log(a === b, a.equals(b));
console.log(new Set([a, b]).size, new Set([a, b].map((m) => m.key())).size);
console.log(`${a}`, String({}));
```

```output for=equals-by-convention
false true
2 1
12.50 EUR [object Object]
```

- Verdict: absent. `Object.prototype` gives `toString` and `valueOf`, but no method for equality or hashing. `===` compares two objects by identity, and code cannot change that.
- An `equals` method is only a convention: `Set`, `Map` and `===` do not call it. To use value equality in a `Set` or as a `Map` key, convert each object to a primitive key, as `key()` does.
- A template literal and `String()` call `toString`. Without an override, the result is `[object Object]`.

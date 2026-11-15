```javascript run id=money-allocate
const FRACTION_DIGITS = { EUR: 2, USD: 2, JPY: 0 }; // from ISO 4217

class Money {
  // units: an integer in the minor unit of the currency, such as cents.
  constructor(units, currency) {
    if (!Number.isSafeInteger(units)) throw new Error("units must be an integer");
    this.units = units;
    this.currency = currency;
    Object.freeze(this);
  }

  plus(other) {
    if (this.currency !== other.currency) {
      throw new Error(`cannot add ${this.currency} and ${other.currency}`);
    }
    return new Money(this.units + other.units, this.currency);
  }

  equals(other) {
    return this.units === other.units && this.currency === other.currency;
  }

  // Split by ratios. The sum of the parts is always the whole amount.
  allocate(ratios) {
    const total = ratios.reduce((s, r) => s + r, 0);
    const parts = ratios.map((r) => Math.floor((this.units * r) / total));
    let left = this.units - parts.reduce((s, p) => s + p, 0);
    for (let i = 0; left > 0; i++, left--) parts[i] += 1; // the units that are left, one each
    return parts.map((p) => new Money(p, this.currency));
  }

  toString() {
    const d = FRACTION_DIGITS[this.currency];
    if (d === 0) return `${this.units} ${this.currency}`;
    const scale = 10 ** d;
    const frac = String(this.units % scale).padStart(d, "0");
    return `${Math.floor(this.units / scale)}.${frac} ${this.currency}`;
  }
}

const show = (parts) => parts.join(", ");

const a = new Money(1000, "EUR");
const b = new Money(50, "EUR");
console.log(`a: ${a}`);
console.log(`a + b: ${a.plus(b)}`);
try {
  a.plus(new Money(500, "JPY"));
} catch (e) {
  console.log("error:", e.message);
}
console.log(`fee: ${new Money(500, "JPY")}`);
console.log("5 cents 70/30:", show(new Money(5, "EUR").allocate([70, 30])));
console.log("100 EUR in 3:", show(new Money(10000, "EUR").allocate([1, 1, 1])));
console.log("equal:", new Money(1050, "EUR").equals(a.plus(b)) ? "yes" : "no");
```

```output
a: 10.00 EUR
a + b: 10.50 EUR
error: cannot add EUR and JPY
fee: 500 JPY
5 cents 70/30: 0.04 EUR, 0.01 EUR
100 EUR in 3: 33.34 EUR, 33.33 EUR, 33.33 EUR
equal: yes
```

- Verdict: idiom. JavaScript numbers are binary floating point, so the class keeps an integer number of minor units and checks it with `Number.isSafeInteger`. For larger amounts, use `BigInt`.
- `Intl.NumberFormat` with `style: "currency"` can format an amount for a locale, but it does not do the arithmetic.

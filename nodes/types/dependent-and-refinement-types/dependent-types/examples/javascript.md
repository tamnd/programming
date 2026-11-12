```javascript run id=smart-constructor
// A smart constructor: the only way to get a Percent checks the predicate
class Percent {
  #value;
  constructor(value, key) {
    if (key !== Percent.#key) throw new TypeError("use Percent.of");
    this.#value = value;
  }
  static #key = Symbol("Percent");
  static of(n) {
    if (!Number.isInteger(n) || n < 0 || n > 100) {
      throw new RangeError(`${n} is not an integer from 0 to 100`);
    }
    return new Percent(n, Percent.#key);
  }
  get value() { return this.#value; }
}

function discount(price, pct) { // trusts pct, because a Percent was checked
  if (!(pct instanceof Percent)) throw new TypeError("pct must be a Percent");
  return price - (price * pct.value) / 100;
}

console.log(discount(80, Percent.of(25)));
for (const attempt of [() => Percent.of(120), () => new Percent(120),
                       () => discount(80, 25)]) {
  try {
    attempt();
  } catch (err) {
    console.log(`${err.name}: ${err.message}`);
  }
}
```

```output for=smart-constructor
60
RangeError: 120 is not an integer from 0 to 100
TypeError: use Percent.of
TypeError: pct must be a Percent
```

- Verdict: idiom. JavaScript has no static types. A smart constructor gives the same guarantee at run time: `Percent.of` is the only way to make a `Percent`, because the constructor needs a private key, so every `Percent` holds a valid value. A function that takes a `Percent` checks the class once and then trusts the value. TypeScript adds branded types for the same idea at compile time.

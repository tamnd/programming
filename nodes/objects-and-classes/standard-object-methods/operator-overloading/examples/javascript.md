```javascript run id=methods-not-operators
class Money {
  constructor(cents) {
    this.cents = cents;
  }
  add(other) {
    return new Money(this.cents + other.cents);
  }
  equals(other) {
    return other instanceof Money && this.cents === other.cents;
  }
  toString() {
    return `Money(${this.cents})`;
  }
}

const a = new Money(150);
const b = new Money(250);
console.log(`${a.add(b)}`);
console.log(a.add(b).equals(new Money(400)));
console.log(a + b);
console.log(new Money(1) === new Money(1));
```

```output for=methods-not-operators
Money(400)
true
Money(150)Money(250)
false
```

- Verdict: absent. A class cannot define what `+` or `===` does, so it has methods such as `add` and `equals`.
- `a + b` converts both objects to primitives. Here `toString` gives strings, so `+` joins them.
- `===` on two objects compares references, so two equal values are not `===`.

```javascript run id=duration
class Duration {
  static #zero = null;
  static #token = Symbol("Duration");
  #seconds;

  constructor(seconds, token) {
    if (token !== Duration.#token) {
      throw new TypeError("use Duration.ofSeconds or Duration.ofMinutes");
    }
    this.#seconds = seconds;
  }

  static ofSeconds(seconds) {
    if (seconds === 0) {
      Duration.#zero ??= new Duration(0, Duration.#token);
      return Duration.#zero; // the same instance every time
    }
    return new Duration(seconds, Duration.#token);
  }

  static ofMinutes(minutes) {
    return Duration.ofSeconds(minutes * 60);
  }

  static parse(text) {
    const m = /^(\d+)([sm])$/.exec(text);
    if (m === null) return null; // a constructor cannot return "no object"
    return Duration.ofSeconds(Number(m[1]) * (m[2] === "m" ? 60 : 1));
  }

  toString() {
    return `${this.#seconds} s`;
  }
}

console.log(`ofMinutes(2) = ${Duration.ofMinutes(2)}`);
console.log(`ofSeconds(90) = ${Duration.ofSeconds(90)}`);
console.log(`zero is cached: ${Duration.ofSeconds(0) === Duration.ofMinutes(0)}`);
console.log(`parse("2m") = ${Duration.parse("2m")}`);
console.log(`parse("2h") = ${Duration.parse("2h")}`);
try {
  new Duration(5);
} catch (e) {
  console.log(`new Duration(5): ${e.constructor.name}`);
}
```

```output
ofMinutes(2) = 120 s
ofSeconds(90) = 90 s
zero is cached: true
parse("2m") = 120 s
parse("2h") = null
new Duration(5): TypeError
```

- Verdict: idiom. A JavaScript class has one `constructor`, and the language has no private constructors. The factory methods are `static` methods.
- The example hides the constructor with a private `Symbol` that only the class can pass. Most code does not do this, and documents the factory methods instead.
- The standard library uses the form: `Array.of(7)` makes `[7]`, but `Array(7)` makes an empty array of length 7. The name removes the doubt about one argument.

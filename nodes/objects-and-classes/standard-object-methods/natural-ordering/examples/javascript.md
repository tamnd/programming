```javascript run id=compare-function-for-sort
console.log([10, 9, 1, 2].sort());
console.log([10, 9, 1, 2].sort((a, b) => a - b));

class Version {
  constructor(major, minor) {
    this.major = major;
    this.minor = minor;
  }

  static compare(a, b) {
    return a.major - b.major || a.minor - b.minor;
  }

  toString() {
    return `${this.major}.${this.minor}`;
  }
}

const vs = [new Version(1, 10), new Version(1, 2), new Version(0, 9)];
console.log(vs.sort(Version.compare).join(" "));
```

```output for=compare-function-for-sort
[ 1, 10, 2, 9 ]
[ 1, 2, 9, 10 ]
0.9 1.2 1.10
```

- Verdict: absent. JavaScript objects have no natural order, and `<` on two objects compares their primitive values.
- Without a comparison function, `sort` converts the items to strings and compares those, so `10` comes before `2`.
- The usual idiom is a static `compare(a, b)` that returns a negative number, zero or a positive number, passed to `sort`. The `||` moves to the next field when the first fields are equal.

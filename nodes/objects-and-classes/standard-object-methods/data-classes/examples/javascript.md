```javascript run id=hand-written-value-class
class Version {
  constructor(major, minor) {
    this.major = major;
    this.minor = minor;
    Object.freeze(this);
  }
  equals(other) {
    return (
      other instanceof Version &&
      this.major === other.major &&
      this.minor === other.minor
    );
  }
  with(changes) {
    const v = { ...this, ...changes };
    return new Version(v.major, v.minor);
  }
  toString() {
    return `Version(${this.major}, ${this.minor})`;
  }
}

const a = new Version(1, 4);
console.log(`${a}`);
console.log(a === new Version(1, 4), a.equals(new Version(1, 4)));
console.log(`${a.with({ minor: 5 })}`);
console.log(new Set([a, new Version(1, 4)]).size);
```

```output for=hand-written-value-class
Version(1, 4)
false true
Version(1, 5)
2
```

- Verdict: absent. JavaScript generates no methods from fields, so the class writes `equals`, a copy method and `toString` by hand, and `Object.freeze` makes the fields read-only.
- `===` compares references, so two equal versions are not `===`.
- `Set` and `Map` also compare keys by reference and cannot use `equals`, so the set keeps both objects.

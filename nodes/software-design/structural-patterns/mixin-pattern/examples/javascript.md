```javascript run id=comparable-mixin
// A mixin as a function from a class to a subclass.
const Comparable = (Base) =>
  class extends Base {
    lessThan(other) {
      return this.compare(other) < 0;
    }
    between(lo, hi) {
      return lo.compare(this) <= 0 && this.compare(hi) <= 0;
    }
  };

class Version extends Comparable(Object) {
  constructor(major, minor) {
    super();
    this.major = major;
    this.minor = minor;
  }
  compare(other) {
    return this.major - other.major || this.minor - other.minor;
  }
  toString() {
    return `${this.major}.${this.minor}`;
  }
}

const yes = (flag) => (flag ? "yes" : "no");
const [v12, v110, v20] = [new Version(1, 2), new Version(1, 10), new Version(2, 0)];
console.log(`${v12} < ${v110}: ${yes(v12.lessThan(v110))}`);
console.log(`${v110} between ${v12} and ${v20}: ${yes(v110.between(v12, v20))}`);
console.log(`${v20} between ${v12} and ${v110}: ${yes(v20.between(v12, v110))}`);
```

```output
1.2 < 1.10: yes
1.10 between 1.2 and 2.0: yes
2.0 between 1.2 and 1.10: no
```

- Verdict: idiom. A function that takes a class and returns a subclass can be applied several times, as in `Comparable(Serializable(Base))`. `Object.assign(Version.prototype, methods)` is the older form, which copies the methods.

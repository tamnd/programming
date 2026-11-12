```javascript run id=containers
class Spec {
  constructor(test) {
    this.test = test;
  }
  isSatisfiedBy(c) {
    return this.test(c);
  }
  and(other) {
    return new Spec((c) => this.isSatisfiedBy(c) && other.isSatisfiedBy(c));
  }
  or(other) {
    return new Spec((c) => this.isSatisfiedBy(c) || other.isSatisfiedBy(c));
  }
  not() {
    return new Spec((c) => !this.isSatisfiedBy(c));
  }
}

const refrigerated = new Spec((c) => c.refrigerated);
const clean = new Spec((c) => c.clean);

const containers = [
  { name: "c1", refrigerated: true, clean: true },
  { name: "c2", refrigerated: true, clean: false },
  { name: "c3", refrigerated: false, clean: true },
];

const select = (spec) =>
  containers.filter((c) => spec.isSatisfiedBy(c)).map((c) => c.name).join(", ");

console.log("meat:", select(refrigerated.and(clean)));
console.log("not refrigerated:", select(refrigerated.not()));
console.log("refrigerated or clean:", select(refrigerated.or(clean)));
```

```output
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

- Verdict: idiom. Each specification wraps a test function, and `and`, `or` and `not` return new specifications.

```typescript run id=containers
interface Container {
  name: string;
  refrigerated: boolean;
  clean: boolean;
}

class Spec<T> {
  private test: (candidate: T) => boolean;

  constructor(test: (candidate: T) => boolean) {
    this.test = test;
  }
  isSatisfiedBy(candidate: T): boolean {
    return this.test(candidate);
  }
  and(other: Spec<T>): Spec<T> {
    return new Spec((c) => this.isSatisfiedBy(c) && other.isSatisfiedBy(c));
  }
  or(other: Spec<T>): Spec<T> {
    return new Spec((c) => this.isSatisfiedBy(c) || other.isSatisfiedBy(c));
  }
  not(): Spec<T> {
    return new Spec((c) => !this.isSatisfiedBy(c));
  }
}

const refrigerated = new Spec<Container>((c) => c.refrigerated);
const clean = new Spec<Container>((c) => c.clean);

const containers: Container[] = [
  { name: "c1", refrigerated: true, clean: true },
  { name: "c2", refrigerated: true, clean: false },
  { name: "c3", refrigerated: false, clean: true },
];

function select(spec: Spec<Container>): string {
  return containers.filter((c) => spec.isSatisfiedBy(c)).map((c) => c.name).join(", ");
}

console.log("meat:", select(refrigerated.and(clean)));
console.log("not refrigerated:", select(refrigerated.not()));
console.log("refrigerated or clean:", select(refrigerated.or(clean)));
```

```output
meat: c1
not refrigerated: c3
refrigerated or clean: c1, c2, c3
```

- Verdict: idiom. `Spec<T>` wraps a test function and is generic in the type of the candidate.

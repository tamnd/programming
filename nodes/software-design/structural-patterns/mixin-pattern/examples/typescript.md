```typescript run id=comparable-mixin
type Constructor<T = {}> = new (...args: any[]) => T;

interface HasCompare {
  compare(other: this): number;
}

function Comparable<TBase extends Constructor<HasCompare>>(Base: TBase) {
  return class extends Base {
    lessThan(other: this): boolean {
      return this.compare(other) < 0;
    }
    between(lo: this, hi: this): boolean {
      return lo.compare(this) <= 0 && this.compare(hi) <= 0;
    }
  };
}

class VersionBase {
  major: number;
  minor: number;
  constructor(major: number, minor: number) {
    this.major = major;
    this.minor = minor;
  }
  compare(other: this): number {
    return this.major - other.major || this.minor - other.minor;
  }
  toString(): string {
    return `${this.major}.${this.minor}`;
  }
}

const Version = Comparable(VersionBase);

const yes = (flag: boolean): string => (flag ? "yes" : "no");
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

- Verdict: idiom. The constraint `Constructor<HasCompare>` makes the compiler check that the base class supplies `compare`, which the mixin requires.

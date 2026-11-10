```typescript run id=repository
interface Repository {
  get(key: string): string;
  name(): string;
}

class Store implements Repository {
  get(key: string): string {
    console.log(`store: load ${key}`);
    return `value-${key}`;
  }

  name(): string {
    return "store";
  }
}

class Logged implements Repository {
  private readonly inner: Repository;

  constructor(inner: Repository) {
    this.inner = inner;
  }

  get(key: string): string {
    console.log(`log: get ${key}`);
    return this.inner.get(key);
  }

  name(): string {
    return this.inner.name();
  }
}

class Cached implements Repository {
  private readonly inner: Repository;
  private readonly cache = new Map<string, string>();

  constructor(inner: Repository) {
    this.inner = inner;
  }

  get(key: string): string {
    let value = this.cache.get(key);
    if (value === undefined) {
      value = this.inner.get(key);
      this.cache.set(key, value);
    }
    return value;
  }

  name(): string {
    return this.inner.name();
  }
}

const repo: Repository = new Logged(new Cached(new Store()));
for (const key of ["a", "a", "b"]) {
  console.log(repo.get(key));
}
console.log(`name: ${repo.name()}`);
```

```output
log: get a
store: load a
value-a
log: get a
value-a
log: get b
store: load b
value-b
name: store
```

- Verdict: idiom. The decorators implement `Repository` and hold a `Repository`. If a decorator leaves out `name`, the compiler reports that the class does not implement the interface.
- The types are structural, so `implements` is a check, not a requirement. An object literal with `get` and `name` is also a `Repository`.
- The fields are assigned in the constructor body. Node runs this file by removing the types, and that mode does not accept parameter properties such as `constructor(private inner: Repository)`.
- TypeScript decorators (`@name` on a class or a member) are a different construct with the same name.

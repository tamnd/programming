```typescript run id=erased
class Box<T> {
  value: T;

  constructor(value: T) {
    this.value = value;
  }
}

function isBoxOf<T>(value: unknown, check: (x: unknown) => x is T): value is Box<T> {
  return value instanceof Box && check(value.value);
}

const isString = (x: unknown): x is string => typeof x === "string";

const a: unknown = new Box("text");
const b: unknown = new Box(42);
console.log(a instanceof Box, b instanceof Box);
console.log(isBoxOf(a, isString), isBoxOf(b, isString));
```

```output for=erased
true true
true false
```

- Verdict: native. The compiler removes all types, so the JavaScript that runs has no `T`.
- A class is also a value, so `instanceof Box` works at run time. The test cannot see the type argument, so both boxes pass it.
- To check a type argument, pass a function that checks a value, such as the type guard `isString`.
- This runner strips the types and does not type-check, so a type error here does not stop the run.

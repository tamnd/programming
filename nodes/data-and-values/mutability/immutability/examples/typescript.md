```typescript run id=readonly
interface Point {
  readonly x: number;
  readonly y: number;
}

const p: Point = { x: 1, y: 2 };
const moved: Point = { ...p, y: 5 };
console.log(p, moved);

const names: ReadonlyArray<string> = ["grace", "ada"];
const sorted = [...names].sort();
console.log(sorted, names);

const settings = { mode: "strict" } as const;
console.log(settings.mode);
```

```output for=readonly
{ x: 1, y: 2 } { x: 1, y: 5 }
[ 'ada', 'grace' ] [ 'grace', 'ada' ]
strict
```

- Verdict: native. `readonly` properties, `ReadonlyArray<T>` and `as const` are checks of the type checker. An assignment to `p.x` is the error TS2540, "Cannot assign to 'x' because it is a read-only property".
- The checks have no effect at run time. The emitted JavaScript can still change the object. Use `Object.freeze` for a run-time guard.
- A `readonly` type does not stop a change through a writable alias, because the checker ignores `readonly` when it compares two types:

```typescript wrong id=alias
interface Person {
  name: string;
  age: number;
}
interface ReadonlyPerson {
  readonly name: string;
  readonly age: number;
}

const writable: Person = { name: "Ada", age: 36 };
const view: ReadonlyPerson = writable;
writable.age++;
console.log(view.age);
```

```output for=alias
37
```

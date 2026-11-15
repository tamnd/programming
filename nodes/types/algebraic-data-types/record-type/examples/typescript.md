```typescript run id=record
type Person = { readonly name: string; readonly age: number };

const yn = (b: boolean): string => (b ? "yes" : "no");

const ada: Person = { name: "Ada", age: 36 };
console.log(`name: ${ada.name} age: ${ada.age}`);

const older: Person = { ...ada, age: ada.age + 1 };
console.log("copy: " + JSON.stringify(older));
console.log("original: " + JSON.stringify(ada));

const other: Person = { name: "Ada", age: 36 };
const samePerson = (a: Person, b: Person): boolean => a.name === b.name && a.age === b.age;
console.log("same fields equal: " + yn(samePerson(ada, other)));
console.log("same fields, ===: " + yn(ada === other));
```

```output for=record
name: Ada age: 36
copy: {"name":"Ada","age":37}
original: {"name":"Ada","age":36}
same fields equal: yes
same fields, ===: no
```

- Verdict: native. An object type with `readonly` fields describes an immutable record. The type checker refuses `ada.age = 40`.
- `readonly` exists only at type-checking time. The program at run time is plain JavaScript.
- Object types are structural: any object with a `name` string and an `age` number is a `Person`.

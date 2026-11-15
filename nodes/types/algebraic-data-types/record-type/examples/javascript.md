```javascript run id=record
const yn = (b) => (b ? "yes" : "no");

const ada = Object.freeze({ name: "Ada", age: 36 });
console.log("name:", ada.name, "age:", ada.age);

const older = { ...ada, age: ada.age + 1 };
console.log("copy:", JSON.stringify(older));
console.log("original:", JSON.stringify(ada));

const other = { name: "Ada", age: 36 };
const samePerson = (a, b) => a.name === b.name && a.age === b.age;
console.log("same fields equal:", yn(samePerson(ada, other)));
console.log("same fields, ===:", yn(ada === other));
```

```output for=record
name: Ada age: 36
copy: {"name":"Ada","age":37}
original: {"name":"Ada","age":36}
same fields equal: yes
same fields, ===: no
```

- Verdict: idiom. A plain object is a record of properties. `Object.freeze` stops later changes to the properties.
- The spread `...ada` copies the properties into a new object, and the later `age` overrides the copied one.
- `===` compares identity, so record equality needs a function that compares the fields.

```javascript run id=structured-clone
let x = 1;
let y = x; // a primitive is copied
y = 2;
console.log(x, y);

const a = { name: "Ann", tags: ["admin"] };
const b = a; // an object is shared
b.name = "Bob";
console.log(a.name, a === b);

const shallow = { ...a }; // copies the top level only
shallow.tags.push("dev");
console.log(a.tags.join(","), shallow === a);

const deep = structuredClone(a); // copies every level
deep.tags.push("ops");
console.log(a.tags.join(","), deep.tags.join(","));
```

```output for=structured-clone
1 2
Bob true
admin,dev false
admin,dev admin,dev,ops
```

- Verdict: native. A number is a primitive, so `y = x` copies it. An object is shared, so `b = a` gives two names for one object. Spread syntax makes a shallow copy: the top-level object is new, but the `tags` array is still shared. `structuredClone` makes a deep copy, so a change to its array does not reach the original.

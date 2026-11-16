```javascript run id=spread-and-structured-clone
const team = { name: "red", members: ["Ann", "Bo"], made: new Date(0) };
const shallow = { ...team };
const deep = structuredClone(team);

team.members.push("Cy");
console.log(shallow.members);
console.log(deep.members);
console.log(deep.made instanceof Date, deep.made === team.made);

class Point {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
}
const p = structuredClone(new Point(1, 2));
console.log(p instanceof Point, p.x, p.y);
```

```output for=spread-and-structured-clone
[ 'Ann', 'Bo', 'Cy' ]
[ 'Ann', 'Bo' ]
true false
false 1 2
```

- Verdict: native. Spread syntax copies the own properties of an object into a new object; the values are not copied, so the copy is shallow.
- `structuredClone` copies plain data deeply, and it knows built-in types such as `Date`, `Map` and `Set`.
- It does not keep the class: the copy of a `Point` is a plain object with the fields `x` and `y`, so a class needs its own copy method.

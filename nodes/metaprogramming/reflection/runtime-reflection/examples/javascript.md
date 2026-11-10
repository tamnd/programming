```javascript run id=inspect
class Point {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
  sum() {
    return this.x + this.y;
  }
}

const p = new Point(3, 4);
console.log("constructor:", p.constructor.name);
console.log("own keys:", Reflect.ownKeys(p));
console.log("methods:", Object.getOwnPropertyNames(Object.getPrototypeOf(p)));
for (const key of Object.keys(p)) {
  console.log(key, typeof p[key], Reflect.get(p, key));
}
console.log("sum() =", Reflect.apply(p.sum, p, []));
console.log("has z:", Reflect.has(p, "z"));
```

```output for=inspect
constructor: Point
own keys: [ 'x', 'y' ]
methods: [ 'constructor', 'sum' ]
x number 3
y number 4
sum() = 7
has z: false
```

An object is a table of properties, so reflection reads the table. `Reflect.ownKeys` lists the keys of the object itself, and the methods of a class live on its prototype.

```javascript wrong id=typo
const point = { x: 3, y: 4 };
const key = "X";
console.log(point.x + Reflect.get(point, key));
```

```output for=typo
NaN
```

A key that does not exist gives `undefined`, not an error. `3 + undefined` is `NaN`, and the program goes on with a wrong value.

- Verdict: native. `typeof`, `Object.keys`, `Object.getPrototypeOf` and the `Reflect` object inspect and change objects at run time. `Reflect` is an ordinary object whose functions match the internal methods of objects.
- JavaScript values have no declared types. Reflection sees properties and prototypes, not types.

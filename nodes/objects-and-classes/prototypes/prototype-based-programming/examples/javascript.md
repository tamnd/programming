```javascript run id=prototype-chain
const pointTraits = {  // behavior shared by all points
  describe() { return `(${this.x}, ${this.y})`; },
  moved(dx, dy) { return Object.assign(Object.create(pointTraits), { x: this.x + dx, y: this.y + dy }); },
};

const origin = Object.create(pointTraits);  // an object whose prototype is pointTraits
origin.x = 0;
origin.y = 0;

const p = Object.create(origin);  // p delegates to origin
p.x = 3;                          // an own property shadows origin.x
console.log(p.describe(), Object.keys(p), Object.getPrototypeOf(p) === origin);

pointTraits.describe = function () { return `<${this.x} ${this.y}>`; };
console.log(p.describe(), p.moved(1, 1).describe());  // one change, every point sees it

// Class syntax builds the same structure: methods live on C.prototype.
class Point {
  constructor(x, y) { this.x = x; this.y = y; }
  describe() { return `[${this.x} ${this.y}]`; }
}
const r = new Point(1, 2);
console.log(typeof Point, Object.getPrototypeOf(r) === Point.prototype,
  Object.hasOwn(r, "describe"), r.describe());

Object.setPrototypeOf(r, pointTraits);  // change the prototype of one object
console.log(r.describe());
```

```output for=prototype-chain
(3, 0) [ 'x' ] true
<3 0> <4 1>
function true false [1 2]
<1 2>
```

```javascript wrong id=shared-array
const formProto = { fields: [] };  // one array, on the prototype

const login = Object.create(formProto);
const signup = Object.create(formProto);
login.fields.push("password");  // reads fields from the prototype, then changes it
signup.fields.push("email");

console.log(login.fields, signup.fields);
```

```output for=shared-array
[ 'password', 'email' ] [ 'password', 'email' ]
```

- Verdict: native. Every object has a prototype, another object or `null`, and a property read walks the chain of prototypes until it finds the name. `Object.create` makes an object with a given prototype, and `Object.getPrototypeOf` and `Object.setPrototypeOf` read and change the link. A write creates an own property that shadows the prototype's. Class syntax builds the same structure: a class is a constructor function, its methods live on its `prototype` object, and each instance delegates to that object.
- The wrong block puts an array on the prototype. `login.fields.push` reads `fields`, finds it on the prototype and changes that one array, because only an assignment to `login.fields` would create an own property. Both objects then show both fields. Keep mutable state in own properties, set in a constructor or a factory function, and keep only methods and constants on the prototype.

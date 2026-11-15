```javascript run id=accessor-properties
"use strict";

class Rectangle {
  #width; // a private field
  height = 0; // a public field with an initial value

  constructor(width, height) {
    this.width = width; // goes through the setter
    this.height = height;
  }

  get width() {
    return this.#width;
  }

  set width(value) {
    if (value < 0) throw new RangeError("negative width");
    this.#width = value;
  }

  get area() {
    return this.#width * this.height;
  }
}

const r = new Rectangle(3, 4);
console.log(r.width, r.area);
r.width = 5;
console.log(r.area);
console.log(Object.keys(r)); // own fields only; the accessors are on the prototype
try {
  r.width = -1;
} catch (e) {
  console.log(e.name);
}
try {
  r.area = 1; // a getter without a setter
} catch (e) {
  console.log(e.name);
}
```

```output for=accessor-properties
3 12
20
[ 'height' ]
RangeError
TypeError
```

- Verdict: native. A field declaration such as `height = 0` gives each instance its own property. `get` and `set` define an accessor property on the prototype, so a read or an assignment calls a function.
- A field whose name starts with `#` is private to the class body, and the accessors give controlled access to it. `Object.keys` lists only `height`, because `#width` is private and the accessors belong to the prototype.
- Assigning to a property that has a getter and no setter throws a `TypeError` in strict mode, which class bodies always use; outside strict mode the assignment is ignored without an error.

```javascript run id=this-binding
"use strict";

class Rectangle {
  constructor(width, height) {
    this.width = width;
    this.height = height;
  }

  area() {
    return this.width * this.height;
  }
}

const r = new Rectangle(3, 4);
console.log(r.area());
console.log(Rectangle.prototype.area.call(r)); // pass the receiver by hand

const bound = r.area.bind(r);
console.log(bound());

const arrow = () => r.area(); // an arrow function keeps r in a closure
console.log(arrow());

const lost = r.area; // the function only: the receiver is lost
try {
  lost();
} catch (e) {
  console.log(e.constructor.name);
}
```

```output for=this-binding
12
12
12
12
TypeError
```

- Verdict: native. Methods live on the prototype of the class, and `this` is the receiver. JavaScript sets `this` from the form of each call: in `r.area()` it is `r`.
- A method read without a call is the plain function. Called alone, as `lost()`, it gets `this` as `undefined` in strict mode, and class bodies are always strict, so `this.width` throws a `TypeError`.
- `bind` makes a function with a fixed `this`. An arrow function that calls the method is the other common fix, for example for event handlers and callbacks.

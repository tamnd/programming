```javascript run id=language-features
// A factory function returns a new object; no class is needed.
function makePoint(x, y) {
  return {
    x,
    y,
    norm() {
      return Math.hypot(this.x, this.y);
    },
  };
}
console.log("factory function:", makePoint(3, 4).norm());

// Object.create makes a new object that has an existing object as its prototype.
const base = {
  greet() {
    return `hello from ${this.name}`;
  },
};
const ann = Object.create(base);
ann.name = "ann";
console.log("prototype:", ann.greet(), Object.getPrototypeOf(ann) === base);

// A class is a value, so code can receive the class that it must create.
class Circle {
  constructor(size) {
    this.size = size;
  }
}
class Square {
  constructor(size) {
    this.size = size;
  }
}
const create = (Kind, size) => new Kind(size);
console.log("class as value:", [Circle, Square].map((K) => create(K, 2).constructor.name).join(" "));

// Defaults and the spread syntax do the work of a builder.
const defaults = { method: "GET", timeout: 30, retries: 0 };
const request = { url: "https://example.com/", ...defaults, retries: 2 };
console.log("builder:", JSON.stringify(request));
```

```output
factory function: 5
prototype: hello from ann true
class as value: Circle Square
builder: {"url":"https://example.com/","method":"GET","timeout":30,"retries":2}
```

- Verdict: idiom. An object literal in a function is a factory, and `Object.create` makes an object from a prototype, which is the model of objects in JavaScript. Classes are values, so the code that creates can take the class as an argument. An options object with defaults and the spread syntax replaces most builders.

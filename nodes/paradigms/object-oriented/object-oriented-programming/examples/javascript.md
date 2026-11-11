```javascript run id=prototypes
class Counter {
  #count = 0;

  increment() {
    this.#count += 1;
    return this;
  }

  get value() {
    return this.#count;
  }
}

class StepCounter extends Counter {
  constructor(step) {
    super();
    this.step = step;
  }

  increment() {
    for (let i = 0; i < this.step; i++) super.increment();
    return this;
  }
}

const c = new StepCounter(5).increment().increment();
console.log(c.value);
console.log(Object.getPrototypeOf(c) === StepCounter.prototype);
console.log(Object.getPrototypeOf(StepCounter.prototype) === Counter.prototype);
console.log(typeof StepCounter);

const plain = { greet() { return "hello from " + this.name; } };
const obj = Object.create(plain);
obj.name = "obj";
console.log(obj.greet(), Object.hasOwn(obj, "greet"));
```

```output for=prototypes
10
true
true
function
hello from obj false
```

- Verdict: native. JavaScript objects inherit from other objects through a prototype chain. When a property is not on the object, the engine looks at its prototype, then at the prototype of the prototype, and so on.
- The `class` syntax builds the same chain. A class is a function, and its methods live on the object in its `prototype` property. Fields that start with `#` are private, and the engine rejects access from outside the class body.
- `Object.create` makes an object with a given prototype and no class at all.

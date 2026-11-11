```javascript run id=prototypes
// Cases are classes; each operation is a method on the prototype.
class Num {
  constructor(value) { this.value = value; }
  evaluate() { return this.value; }
}

class Add {
  constructor(left, right) { this.left = left; this.right = right; }
  evaluate() { return this.left.evaluate() + this.right.evaluate(); }
}

// A new operation, added later from another module, without editing the classes.
const show = Symbol("show");
Num.prototype[show] = function () { return String(this.value); };
Add.prototype[show] = function () {
  return `(${this.left[show]()} + ${this.right[show]()})`;
};

// A new case that supplies both operations.
class Neg {
  constructor(inner) { this.inner = inner; }
  evaluate() { return -this.inner.evaluate(); }
  [show]() { return `-${this.inner[show]()}`; }
}

const e = new Add(new Num(1), new Neg(new Add(new Num(2), new Num(3))));
console.log(e[show](), "=", e.evaluate());

class Mul {  // a new case that does not know about show
  constructor(left, right) { this.left = left; this.right = right; }
  evaluate() { return this.left.evaluate() * this.right.evaluate(); }
}
try {
  new Add(new Num(1), new Mul(new Num(2), new Num(3)))[show]();
} catch (err) {
  console.log(err.constructor.name, err.message);
}
```

```output for=prototypes
(1 + -(2 + 3)) = -4
TypeError this.right[show] is not a function
```

```javascript wrong id=switch-falls-through
// Cases are plain objects with a tag; each operation is a switch over the tags.
function evaluate(e) {
  switch (e.kind) {
    case "num": return e.value;
    case "add": return evaluate(e.left) + evaluate(e.right);
  }
}

const num = (value) => ({ kind: "num", value });
const add = (left, right) => ({ kind: "add", left, right });
const neg = (inner) => ({ kind: "neg", inner });  // a case added later

console.log(evaluate(add(num(1), neg(num(5)))));
```

```output for=switch-falls-through
NaN
```

- Verdict: idiom. JavaScript objects are open: a module can add a method to the prototype of a class that another module defined, so new operations and new cases both need no edit to existing code. A symbol as the method key keeps the new operation from clashing with names that other code adds. A case that lacks the operation fails only when the operation reaches it, with a `TypeError`.
- The wrong block uses objects with a `kind` tag and a `switch` in each operation. The new `neg` case has no branch, so `evaluate` returns `undefined`, and adding it to a number gives `NaN` with no error. Add a `default` branch that throws, or use TypeScript, which can check that a `switch` covers every member of a union.

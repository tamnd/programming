```javascript run id=expr
class UnknownVariable extends Error {}

// Each class is one rule of the grammar. Each one evaluates and shows itself.
class Num {
  constructor(value) { this.value = value; }
  evaluate(env) { return this.value; }
  show() { return String(this.value); }
}

class Var {
  constructor(name) { this.name = name; }
  evaluate(env) {
    if (!env.has(this.name)) throw new UnknownVariable(`unknown variable ${this.name}`);
    return env.get(this.name);
  }
  show() { return this.name; }
}

class Add {
  constructor(left, right) { this.left = left; this.right = right; }
  evaluate(env) { return this.left.evaluate(env) + this.right.evaluate(env); }
  show() { return `${this.left.show()} + ${this.right.show()}`; }
}

class Mul {
  constructor(left, right) { this.left = left; this.right = right; }
  evaluate(env) { return this.left.evaluate(env) * this.right.evaluate(env); }
  show() { return `${wrap(this.left)} * ${wrap(this.right)}`; }
}

// A sum inside a product needs parentheses.
const wrap = (e) => (e instanceof Add ? `(${e.show()})` : e.show());

function report(e, env) {
  const label = [...env].map(([k, v]) => `${k}=${v}`).join(" ");
  try {
    console.log(`${label}: ${e.evaluate(env)}`);
  } catch (err) {
    if (!(err instanceof UnknownVariable)) throw err;
    console.log(`${label}: error: ${err.message}`);
  }
}

const e = new Mul(new Var("x"), new Add(new Var("y"), new Num(2)));
console.log("expression:", e.show());
report(e, new Map([["x", 3], ["y", 4]]));
report(e, new Map([["x", 5], ["y", 1]]));

const f = new Add(new Var("z"), new Num(1));
console.log("expression:", f.show());
report(f, new Map([["x", 3], ["y", 4]]));
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```

- Verdict: idiom. JavaScript has no construct for the pattern. The classes need no common base class, because the methods are found at run time.
- The `Map` is the Context. A `Map` is used, not a plain object, so that a variable called `constructor` or `toString` is not found on the prototype.
- The error class lets `report` catch only the errors of the language and let real bugs pass.

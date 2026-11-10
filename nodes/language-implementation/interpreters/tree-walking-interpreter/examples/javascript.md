```javascript run id=interpreter
const num = (value) => ({ kind: "num", value });
const v = (name) => ({ kind: "var", name });
const bin = (op, left, right) => ({ kind: "bin", op, left, right });
const let_ = (name, init) => ({ kind: "let", name, init });
const assign = (name, value) => ({ kind: "assign", name, value });
const block = (...body) => ({ kind: "block", body });
const while_ = (cond, body) => ({ kind: "while", cond, body });
const print = (value) => ({ kind: "print", value });

class Env {
  constructor(parent = null) {
    this.vars = new Map();
    this.parent = parent;
  }
}

class Interpreter {
  nodes = 0;
  lookups = 0;
  links = 0;

  find(env, name) {
    this.lookups++;
    for (let e = env; e !== null; e = e.parent) {
      if (e.vars.has(name)) return e;
      this.links++;
    }
    throw new ReferenceError(`undefined variable '${name}'`);
  }

  evaluate(node, env) {
    this.nodes++;
    switch (node.kind) {
      case "num":
        return node.value;
      case "var":
        return this.find(env, node.name).vars.get(node.name);
      case "bin": {
        const a = this.evaluate(node.left, env);
        const b = this.evaluate(node.right, env);
        switch (node.op) {
          case "+": return a + b;
          case "-": return a - b;
          case "*": return a * b;
          case "<": return a < b ? 1 : 0;
        }
      }
    }
    throw new TypeError(`unknown expression ${node.kind}`);
  }

  execute(node, env) {
    this.nodes++;
    switch (node.kind) {
      case "let":
        env.vars.set(node.name, this.evaluate(node.init, env));
        return;
      case "assign": {
        const result = this.evaluate(node.value, env);
        this.find(env, node.name).vars.set(node.name, result);
        return;
      }
      case "block": {
        const inner = new Env(env);
        for (const stmt of node.body) this.execute(stmt, inner);
        return;
      }
      case "while":
        while (this.evaluate(node.cond, env) !== 0) this.execute(node.body, env);
        return;
      case "print":
        console.log(this.evaluate(node.value, env));
        return;
    }
    throw new TypeError(`unknown statement ${node.kind}`);
  }
}

const program = block(
  let_("n", num(5)),
  let_("acc", num(1)),
  while_(bin("<", num(0), v("n")), block(
    let_("step", v("n")),
    assign("acc", bin("*", v("acc"), v("step"))),
    assign("n", bin("-", v("n"), num(1))),
  )),
  print(v("acc")),
);

const interp = new Interpreter();
interp.execute(program, new Env());
console.log(`nodes visited: ${interp.nodes}`);
console.log(`variable lookups: ${interp.lookups}, parent links followed: ${interp.links}`);
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. Nodes are plain objects with a `kind` field, and small factory functions keep the program tree readable.
- `switch` on `node.kind` selects the case. Nothing checks that every kind has a case, so the functions end with a `throw` for an unknown node.
- Each environment holds a `Map` and a reference to its parent. A block that ends drops its environment, and the garbage collector reclaims it.

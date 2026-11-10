```typescript run id=interpreter
type Expr =
  | { kind: "num"; value: number }
  | { kind: "var"; name: string }
  | { kind: "bin"; op: "+" | "-" | "*" | "<"; left: Expr; right: Expr };

type Stmt =
  | { kind: "let"; name: string; init: Expr }
  | { kind: "assign"; name: string; value: Expr }
  | { kind: "block"; body: Stmt[] }
  | { kind: "while"; cond: Expr; body: Stmt }
  | { kind: "print"; value: Expr };

class Env {
  readonly vars = new Map<string, number>();
  readonly parent: Env | null;

  constructor(parent: Env | null = null) {
    this.parent = parent;
  }
}

class Interpreter {
  nodes = 0;
  lookups = 0;
  links = 0;

  private find(env: Env, name: string): Env {
    this.lookups++;
    for (let e: Env | null = env; e !== null; e = e.parent) {
      if (e.vars.has(name)) return e;
      this.links++;
    }
    throw new ReferenceError(`undefined variable '${name}'`);
  }

  evaluate(node: Expr, env: Env): number {
    this.nodes++;
    switch (node.kind) {
      case "num":
        return node.value;
      case "var":
        return this.find(env, node.name).vars.get(node.name)!;
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
  }

  execute(node: Stmt, env: Env): void {
    this.nodes++;
    switch (node.kind) {
      case "let":
        env.vars.set(node.name, this.evaluate(node.init, env));
        break;
      case "assign": {
        const result = this.evaluate(node.value, env);
        this.find(env, node.name).vars.set(node.name, result);
        break;
      }
      case "block": {
        const inner = new Env(env);
        for (const stmt of node.body) this.execute(stmt, inner);
        break;
      }
      case "while":
        while (this.evaluate(node.cond, env) !== 0) this.execute(node.body, env);
        break;
      case "print":
        console.log(this.evaluate(node.value, env));
        break;
      default: {
        const unreachable: never = node;
        throw new TypeError(`unknown statement ${JSON.stringify(unreachable)}`);
      }
    }
  }
}

const program: Stmt = {
  kind: "block",
  body: [
    { kind: "let", name: "n", init: { kind: "num", value: 5 } },
    { kind: "let", name: "acc", init: { kind: "num", value: 1 } },
    {
      kind: "while",
      cond: { kind: "bin", op: "<", left: { kind: "num", value: 0 }, right: { kind: "var", name: "n" } },
      body: {
        kind: "block",
        body: [
          { kind: "let", name: "step", init: { kind: "var", name: "n" } },
          { kind: "assign", name: "acc", value: { kind: "bin", op: "*", left: { kind: "var", name: "acc" }, right: { kind: "var", name: "step" } } },
          { kind: "assign", name: "n", value: { kind: "bin", op: "-", left: { kind: "var", name: "n" }, right: { kind: "num", value: 1 } } },
        ],
      },
    },
    { kind: "print", value: { kind: "var", name: "acc" } },
  ],
};

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

- Verdict: idiom. `Expr` and `Stmt` are discriminated unions, and `switch` on `kind` narrows each case to the right fields.
- In `execute`, the `default` branch assigns `node` to a variable of type `never`. If a new kind of statement is added to `Stmt` without a case, the type checker rejects that line.
- In `evaluate`, the checker sees that the switches cover every case, so the function needs no final `return`. Node.js strips the types and does not check them, so these checks happen only under `tsc`. Type stripping also rejects syntax that needs code generation, such as parameter properties in a constructor, so `Env` assigns `parent` in the body.

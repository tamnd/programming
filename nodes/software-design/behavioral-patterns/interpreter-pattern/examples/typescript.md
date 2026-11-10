```typescript run id=expr
// The grammar as a discriminated union. Each operation is one function.
type Expr =
  | { kind: "num"; value: number }
  | { kind: "var"; name: string }
  | { kind: "add"; left: Expr; right: Expr }
  | { kind: "mul"; left: Expr; right: Expr };

type Env = ReadonlyMap<string, number>;
type Result = { ok: true; value: number } | { ok: false; error: string };

const num = (value: number): Expr => ({ kind: "num", value });
const v = (name: string): Expr => ({ kind: "var", name });
const add = (left: Expr, right: Expr): Expr => ({ kind: "add", left, right });
const mul = (left: Expr, right: Expr): Expr => ({ kind: "mul", left, right });

function evaluate(e: Expr, env: Env): Result {
  switch (e.kind) {
    case "num":
      return { ok: true, value: e.value };
    case "var": {
      const value = env.get(e.name);
      return value === undefined
        ? { ok: false, error: `unknown variable ${e.name}` }
        : { ok: true, value };
    }
    case "add":
    case "mul": {
      const l = evaluate(e.left, env);
      if (!l.ok) return l;
      const r = evaluate(e.right, env);
      if (!r.ok) return r;
      const value = e.kind === "add" ? l.value + r.value : l.value * r.value;
      return { ok: true, value };
    }
    default: {
      const unreachable: never = e;
      return unreachable;
    }
  }
}

function show(e: Expr): string {
  switch (e.kind) {
    case "num":
      return String(e.value);
    case "var":
      return e.name;
    case "add":
      return `${show(e.left)} + ${show(e.right)}`;
    case "mul":
      return `${wrap(e.left)} * ${wrap(e.right)}`;
  }
}

const wrap = (e: Expr): string => (e.kind === "add" ? `(${show(e)})` : show(e));

function report(e: Expr, env: Env): void {
  const label = [...env].map(([k, val]) => `${k}=${val}`).join(" ");
  const r = evaluate(e, env);
  console.log(r.ok ? `${label}: ${r.value}` : `${label}: error: ${r.error}`);
}

const e = mul(v("x"), add(v("y"), num(2)));
console.log("expression:", show(e));
report(e, new Map([["x", 3], ["y", 4]]));
report(e, new Map([["x", 5], ["y", 1]]));

const f = add(v("z"), num(1));
console.log("expression:", show(f));
report(f, new Map([["x", 3], ["y", 4]]));
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```

- Verdict: idiom. The nodes are plain objects with a `kind` tag. The compiler narrows the type in each `case`, so `e.left` is only visible where the node has a `left`.
- The `never` check in `evaluate` makes the compiler reject the function if a fifth kind joins the union and no `case` handles it. `show` gets the same check from its return type: a missing `case` means a path that returns no string.
- This form turns the pattern around. One more operation is one more function, and no node changes. One more kind of node means a change to every function. The Python and JavaScript examples have the opposite trade-off.
- `evaluate` returns a result object instead of throwing, so the type of the function shows that it can fail.

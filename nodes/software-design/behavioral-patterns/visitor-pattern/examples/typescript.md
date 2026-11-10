```typescript run id=expr
type Expr =
  | { kind: "num"; value: number }
  | { kind: "add"; left: Expr; right: Expr }
  | { kind: "mul"; left: Expr; right: Expr };

// A visitor is a record with one function for each kind. The type makes all three required.
type Visitor<R> = {
  num: (value: number) => R;
  add: (left: R, right: R) => R;
  mul: (left: R, right: R) => R;
};

// fold walks the tree once and gives the visitor the results of the children.
function fold<R>(e: Expr, v: Visitor<R>): R {
  switch (e.kind) {
    case "num":
      return v.num(e.value);
    case "add":
      return v.add(fold(e.left, v), fold(e.right, v));
    case "mul":
      return v.mul(fold(e.left, v), fold(e.right, v));
    default: {
      const unknown: never = e;
      throw new Error(`unknown node ${JSON.stringify(unknown)}`);
    }
  }
}

const show: Visitor<string> = {
  num: (value) => String(value),
  add: (l, r) => `(${l} + ${r})`,
  mul: (l, r) => `(${l} * ${r})`,
};

const evaluate: Visitor<number> = {
  num: (value) => value,
  add: (l, r) => l + r,
  mul: (l, r) => l * r,
};

const count: Visitor<number> = {
  num: () => 1,
  add: (l, r) => 1 + l + r,
  mul: (l, r) => 1 + l + r,
};

const num = (value: number): Expr => ({ kind: "num", value });
const tree: Expr = {
  kind: "mul",
  left: { kind: "add", left: num(2), right: num(3) },
  right: num(4),
};

console.log("show:", fold(tree, show));
console.log("evaluate:", fold(tree, evaluate));
console.log("nodes:", fold(tree, count));
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

- Verdict: idiom. The nodes are a discriminated union on the `kind` field, and the visitor is a record of functions. `tsc` rejects a visitor that lacks a function for a kind.
- `fold` holds the traversal, so the visitors do not recurse. Each function receives the results for the children. This answers the question of Design Patterns about who traverses the structure: here the code that owns the structure does.
- The `default` case assigns `e` to a variable of type `never`. If a kind is added to `Expr` without a case, `e` has that kind there, and `tsc` rejects the assignment. The TypeScript handbook calls this exhaustiveness checking. Node strips the types and does not check them, so only `tsc` reports the error.

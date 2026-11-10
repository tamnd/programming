```typescript run id=parse-walk
type Expr =
  | { kind: "num"; value: number }
  | { kind: "var"; name: string }
  | { kind: "bin"; op: "+" | "-" | "*"; left: Expr; right: Expr };

function parse(src: string): Expr {
  let pos = 0;
  const peek = (): string => {
    while (src[pos] === " ") pos++;
    return src[pos] ?? "";
  };

  function expr(): Expr {
    let left = term();
    while (peek() === "+" || peek() === "-") {
      const op = src[pos++] as "+" | "-";
      left = { kind: "bin", op, left, right: term() };
    }
    return left;
  }

  function term(): Expr {
    let left = factor();
    while (peek() === "*") {
      pos++;
      left = { kind: "bin", op: "*", left, right: factor() };
    }
    return left;
  }

  function factor(): Expr {
    const c = peek();
    if (c === "(") {
      pos++;
      const inner = expr();
      if (peek() !== ")") throw new SyntaxError(`expected ) at ${pos}`);
      pos++;
      return inner;
    }
    const m = /^(\d+)|^([a-z]+)/.exec(src.slice(pos));
    if (!m) throw new SyntaxError(`unexpected ${c} at ${pos}`);
    pos += m[0].length;
    return m[1] ? { kind: "num", value: Number(m[1]) } : { kind: "var", name: m[2] };
  }

  return expr();
}

function show(node: Expr, depth = 0): void {
  const pad = "  ".repeat(depth);
  switch (node.kind) {
    case "num":
      console.log(`${pad}Num ${node.value}`);
      break;
    case "var":
      console.log(`${pad}Var ${node.name}`);
      break;
    case "bin":
      console.log(`${pad}Bin ${node.op}`);
      show(node.left, depth + 1);
      show(node.right, depth + 1);
  }
}

function count(node: Expr): number {
  return node.kind === "bin" ? 1 + count(node.left) + count(node.right) : 1;
}

function names(node: Expr): string[] {
  switch (node.kind) {
    case "num":
      return [];
    case "var":
      return [node.name];
    case "bin":
      return [...names(node.left), ...names(node.right)];
  }
}

function evaluate(node: Expr, env: Record<string, number>): number {
  switch (node.kind) {
    case "num":
      return node.value;
    case "var":
      return env[node.name];
    case "bin": {
      const l = evaluate(node.left, env);
      const r = evaluate(node.right, env);
      switch (node.op) {
        case "+":
          return l + r;
        case "-":
          return l - r;
        case "*":
          return l * r;
      }
    }
  }
}

const tree = parse("1 + 2 * (3 - x)");
show(tree);
console.log("nodes:", count(tree));
console.log("names:", names(tree));
console.log("value at x = 1:", evaluate(tree, { x: 1 }));
```

```output for=parse-walk
Bin +
  Num 1
  Bin *
    Num 2
    Bin -
      Num 3
      Var x
nodes: 7
names: [ 'x' ]
value at x = 1: 5
```

- Verdict: idiom. A discriminated union lists the kinds of node. Inside `case "bin"`, the type checker narrows `node` to the variant that has `left` and `right`.
- The TypeScript compiler API (`ts.createSourceFile`, `ts.forEachChild`) parses TypeScript itself. It lives in the `typescript` package, not in a standard library.

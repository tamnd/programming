```javascript run id=parse-walk
function parse(src) {
  let pos = 0;
  const peek = () => {
    while (src[pos] === " ") pos++;
    return src[pos];
  };

  function expr() {
    let left = term();
    while (peek() === "+" || peek() === "-") {
      const op = src[pos++];
      left = { kind: "bin", op, left, right: term() };
    }
    return left;
  }

  function term() {
    let left = factor();
    while (peek() === "*") {
      pos++;
      left = { kind: "bin", op: "*", left, right: factor() };
    }
    return left;
  }

  function factor() {
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

function show(node, depth = 0) {
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

function* nodes(node) {
  yield node;
  if (node.kind === "bin") {
    yield* nodes(node.left);
    yield* nodes(node.right);
  }
}

function evaluate(node, env) {
  switch (node.kind) {
    case "num":
      return node.value;
    case "var":
      return env[node.name];
    case "bin": {
      const l = evaluate(node.left, env);
      const r = evaluate(node.right, env);
      return node.op === "+" ? l + r : node.op === "-" ? l - r : l * r;
    }
  }
}

const tree = parse("1 + 2 * (3 - x)");
show(tree);
const all = [...nodes(tree)];
console.log("nodes:", all.length);
console.log("names:", all.filter((n) => n.kind === "var").map((n) => n.name));
console.log("value at x = 1:", evaluate(tree, { x: 1 }));
console.log(JSON.stringify(tree));
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
{"kind":"bin","op":"+","left":{"kind":"num","value":1},"right":{"kind":"bin","op":"*","left":{"kind":"num","value":2},"right":{"kind":"bin","op":"-","left":{"kind":"num","value":3},"right":{"kind":"var","name":"x"}}}}
```

- Verdict: idiom. Each node is a plain object with a `kind` field, and a `switch` on `kind` selects the case. Nothing stops a node from having the wrong fields; the TypeScript example adds the check.
- A generator function walks the tree. `yield*` hands over to the walk of each child, so the caller gets a flat sequence of nodes in depth-first order.
- A tree of plain objects is also valid JSON. `JSON.stringify` prints it with the nesting of the tree.
- Node.js and the browsers expose no parser for JavaScript itself. A program that needs the AST of JavaScript source uses a parser package from npm.

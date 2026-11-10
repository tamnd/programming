```javascript run id=engine
// Values carry their type, as boxed values do inside a JavaScript engine.
const int = (n) => ({ kind: "int", n });
const float = (x) => ({ kind: "float", x });
const show = (v) => String(v.kind === "int" ? v.n : v.x);

// The body of f(x) = x * x + 1 as a tree.
const num = (value) => ({ type: "num", value });
const arg = { type: "arg" };
const bin = (op, left, right) => ({ type: "bin", op, left, right });

const OPS = { "+": (a, b) => a + b, "*": (a, b) => a * b };

// Generic arithmetic: check both tags on every operation.
function arith(op, a, b) {
  if (a.kind === "int" && b.kind === "int") return int(OPS[op](a.n, b.n));
  const fa = a.kind === "int" ? a.n : a.x;
  const fb = b.kind === "int" ? b.n : b.x;
  return float(OPS[op](fa, fb));
}

// The interpreter: walk the tree on every call.
function interpret(node, x) {
  switch (node.type) {
    case "num": return int(node.value);
    case "arg": return x;
    case "bin": return arith(node.op, interpret(node.left, x), interpret(node.right, x));
  }
}

// Compiled code for int: closures over raw numbers, no tag checks inside.
function compileInt(node) {
  switch (node.type) {
    case "num": { const v = node.value; return () => v; }
    case "arg": return (n) => n;
    case "bin": {
      const f = OPS[node.op], l = compileInt(node.left), r = compileInt(node.right);
      return (n) => f(l(n), r(n));
    }
  }
}

// Compiled code for any argument: closures that still check tags.
function compileGeneric(node) {
  switch (node.type) {
    case "num": { const boxed = int(node.value); return () => boxed; }
    case "arg": return (a) => a;
    case "bin": {
      const op = node.op, l = compileGeneric(node.left), r = compileGeneric(node.right);
      return (a) => arith(op, l(a), r(a));
    }
  }
}

const THRESHOLD = 3;

class JitFunction {
  constructor(name, body) {
    this.name = name;
    this.body = body;
    this.calls = 0; // calls in the interpreter since the last compilation
    this.seen = new Set(); // the profile: the kinds of the arguments
    this.code = null; // compiled code, or null
    this.counts = { interpreter: 0, compiled: 0, deoptimizations: 0 };
  }

  compile() {
    let how;
    if (this.seen.size === 1 && this.seen.has("int")) {
      const raw = compileInt(this.body);
      this.code = (a) => (a.kind === "int" ? int(raw(a.n)) : null); // the guard
      this.kind = "int";
      how = "specialized for int";
    } else {
      this.code = compileGeneric(this.body);
      this.kind = how = "generic";
    }
    console.log(`* compile ${this.name}: arguments seen: ${[...this.seen].sort().join(", ")}; ${how}`);
  }

  call(a) {
    if (this.code === null && this.calls >= THRESHOLD) this.compile();
    if (this.code !== null) {
      const result = this.code(a);
      if (result !== null) {
        this.counts.compiled++;
        return [result, `compiled, ${this.kind}`];
      }
      console.log(`* deoptimize ${this.name}: guard failed, ${show(a)} is not an int`);
      this.counts.deoptimizations++;
      this.code = null;
      this.calls = 0;
    }
    this.calls++;
    this.counts.interpreter++;
    this.seen.add(a.kind);
    return [interpret(this.body, a), `interpreter, call ${this.calls}`];
  }
}

const f = new JitFunction("f", bin("+", bin("*", arg, arg), num(1)));
for (const a of [int(1), int(2), int(3), int(4), float(2.5), int(6), int(7), int(8)]) {
  const [result, tier] = f.call(a);
  console.log(`f(${show(a)}) = ${show(result)} [${tier}]`);
}
console.log(Object.entries(f.counts).map(([k, v]) => `${k}: ${v}`).join(", "));
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

- Verdict: native. V8, the engine of Chrome and Node.js, interprets Ignition bytecode first and compiles hot functions with Sparkplug, Maglev and TurboFan.
- Every JavaScript number is a double in the language, so the model needs its own `kind` tag to tell `int` from `float`. V8 tracks the types of values itself while the program runs.
- The compiled code is a tree of closures. Each closure captures the closures of its children, so a call runs no `switch` on the node type.

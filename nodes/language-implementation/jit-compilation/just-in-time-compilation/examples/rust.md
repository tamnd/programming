```rust run id=engine
use std::collections::BTreeSet;
use std::fmt;

// Values carry their type, as boxed values do in a dynamic language.
#[derive(Clone, Copy)]
enum Value {
    Int(i64),
    Float(f64),
}

impl Value {
    fn kind(self) -> &'static str {
        match self {
            Value::Int(_) => "int",
            Value::Float(_) => "float",
        }
    }
    fn as_f64(self) -> f64 {
        match self {
            Value::Int(n) => n as f64,
            Value::Float(x) => x,
        }
    }
}

impl fmt::Display for Value {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Value::Int(n) => write!(f, "{n}"),
            Value::Float(x) => write!(f, "{x}"),
        }
    }
}

// The body of f(x) = x * x + 1 as a tree.
enum Node {
    Num(i64),
    Arg,
    Bin(char, Box<Node>, Box<Node>),
}

fn int_op(op: char) -> fn(i64, i64) -> i64 {
    if op == '+' {
        |a, b| a + b
    } else {
        |a, b| a * b
    }
}

// Generic arithmetic: check both tags on every operation.
fn arith(op: char, a: Value, b: Value) -> Value {
    match (a, b) {
        (Value::Int(x), Value::Int(y)) => Value::Int(int_op(op)(x, y)),
        _ if op == '+' => Value::Float(a.as_f64() + b.as_f64()),
        _ => Value::Float(a.as_f64() * b.as_f64()),
    }
}

// The interpreter: walk the tree on every call.
fn interpret(node: &Node, arg: Value) -> Value {
    match node {
        Node::Num(v) => Value::Int(*v),
        Node::Arg => arg,
        Node::Bin(op, l, r) => arith(*op, interpret(l, arg), interpret(r, arg)),
    }
}

type IntCode = Box<dyn Fn(i64) -> i64>;
type GenericCode = Box<dyn Fn(Value) -> Value>;

// Compiled code for int: closures over raw i64 values, no tag checks inside.
fn compile_int(node: &Node) -> IntCode {
    match node {
        Node::Num(v) => {
            let v = *v;
            Box::new(move |_| v)
        }
        Node::Arg => Box::new(|x| x),
        Node::Bin(op, l, r) => {
            let (f, l, r) = (int_op(*op), compile_int(l), compile_int(r));
            Box::new(move |x| f(l(x), r(x)))
        }
    }
}

// Compiled code for any argument: closures that still check tags.
fn compile_generic(node: &Node) -> GenericCode {
    match node {
        Node::Num(v) => {
            let boxed = Value::Int(*v);
            Box::new(move |_| boxed)
        }
        Node::Arg => Box::new(|a| a),
        Node::Bin(op, l, r) => {
            let (op, l, r) = (*op, compile_generic(l), compile_generic(r));
            Box::new(move |a| arith(op, l(a), r(a)))
        }
    }
}

const THRESHOLD: u32 = 3;

// Compiled code; it returns None when the guard fails.
type Code = Box<dyn Fn(Value) -> Option<Value>>;

struct JitFunction {
    name: String,
    body: Node,
    calls: u32,                   // calls in the interpreter since the last compilation
    seen: BTreeSet<&'static str>, // the profile: the kinds of the arguments
    code: Option<(Code, &'static str)>, // compiled code and its kind
    counts: [u32; 3],             // interpreter, compiled, deoptimizations
}

impl JitFunction {
    fn compile(&mut self) {
        let (code, kind, how): (Code, _, _) = if self.seen.len() == 1 && self.seen.contains("int") {
            let raw = compile_int(&self.body);
            let guarded = move |a| match a {
                Value::Int(n) => Some(Value::Int(raw(n))), // the guard
                Value::Float(_) => None,
            };
            (Box::new(guarded), "int", "specialized for int")
        } else {
            let generic = compile_generic(&self.body);
            (Box::new(move |a| Some(generic(a))), "generic", "generic")
        };
        let seen: Vec<_> = self.seen.iter().copied().collect();
        println!(
            "* compile {}: arguments seen: {}; {how}",
            self.name,
            seen.join(", ")
        );
        self.code = Some((code, kind));
    }

    fn call(&mut self, arg: Value) -> (Value, String) {
        if self.code.is_none() && self.calls >= THRESHOLD {
            self.compile();
        }
        if let Some((code, kind)) = &self.code {
            if let Some(result) = code(arg) {
                self.counts[1] += 1;
                return (result, format!("compiled, {kind}"));
            }
            println!(
                "* deoptimize {}: guard failed, {arg} is not an int",
                self.name
            );
            self.counts[2] += 1;
            self.code = None;
            self.calls = 0;
        }
        self.calls += 1;
        self.counts[0] += 1;
        self.seen.insert(arg.kind());
        (
            interpret(&self.body, arg),
            format!("interpreter, call {}", self.calls),
        )
    }
}

fn main() {
    use Node::*;
    let body = Bin(
        '+',
        Box::new(Bin('*', Box::new(Arg), Box::new(Arg))),
        Box::new(Num(1)),
    );
    let mut f = JitFunction {
        name: "f".into(),
        body,
        calls: 0,
        seen: BTreeSet::new(),
        code: None,
        counts: [0; 3],
    };
    let inputs = [1, 2, 3, 4]
        .map(Value::Int)
        .into_iter()
        .chain([Value::Float(2.5)])
        .chain([6, 7, 8].map(Value::Int));
    for arg in inputs {
        let (result, tier) = f.call(arg);
        println!("f({arg}) = {result} [{tier}]");
    }
    let [i, c, d] = f.counts;
    println!("interpreter: {i}, compiled: {c}, deoptimizations: {d}");
}
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

- Verdict: idiom. Rust compiles ahead of time to machine code, so a JIT is something a program builds, as here.
- `Value` is an `enum`, a sum type. The guard is a `match` on the variant, and the compiler rejects a `match` that leaves a variant out.
- Compiled code is a `Box<dyn Fn>`, a closure on the heap with its captured children. `move` gives each closure ownership of what it captures.
- `BTreeSet` keeps the profile sorted, so the log prints the kinds in the same order on every run.

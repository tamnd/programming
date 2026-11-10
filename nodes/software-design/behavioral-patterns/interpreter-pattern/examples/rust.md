```rust run id=expr
use std::collections::HashMap;
use std::fmt;

// The grammar as an enum. A Box gives the recursive variants a fixed size.
enum Expr {
    Num(i64),
    Var(&'static str),
    Add(Box<Expr>, Box<Expr>),
    Mul(Box<Expr>, Box<Expr>),
}

use Expr::*;

fn evaluate(e: &Expr, env: &HashMap<&str, i64>) -> Result<i64, String> {
    match e {
        Num(n) => Ok(*n),
        Var(name) => env
            .get(name)
            .copied()
            .ok_or_else(|| format!("unknown variable {name}")),
        Add(l, r) => Ok(evaluate(l, env)? + evaluate(r, env)?),
        Mul(l, r) => Ok(evaluate(l, env)? * evaluate(r, env)?),
    }
}

impl fmt::Display for Expr {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Num(n) => write!(f, "{n}"),
            Var(name) => write!(f, "{name}"),
            Add(l, r) => write!(f, "{l} + {r}"),
            Mul(l, r) => write!(f, "{} * {}", Wrap(l), Wrap(r)),
        }
    }
}

// A sum inside a product needs parentheses.
struct Wrap<'a>(&'a Expr);

impl fmt::Display for Wrap<'_> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self.0 {
            Add(..) => write!(f, "({})", self.0),
            other => write!(f, "{other}"),
        }
    }
}

fn report(e: &Expr, bindings: &[(&'static str, i64)]) {
    let env: HashMap<&str, i64> = bindings.iter().copied().collect();
    let label: Vec<String> = bindings.iter().map(|(k, v)| format!("{k}={v}")).collect();
    match evaluate(e, &env) {
        Ok(value) => println!("{}: {value}", label.join(" ")),
        Err(error) => println!("{}: error: {error}", label.join(" ")),
    }
}

fn main() {
    let e = Mul(Box::new(Var("x")), Box::new(Add(Box::new(Var("y")), Box::new(Num(2)))));
    println!("expression: {e}");
    report(&e, &[("x", 3), ("y", 4)]);
    report(&e, &[("x", 5), ("y", 1)]);

    let f = Add(Box::new(Var("z")), Box::new(Num(1)));
    println!("expression: {f}");
    report(&f, &[("x", 3), ("y", 4)]);
}
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```

- Verdict: idiom. An enum gives the rules of the grammar and `match` gives each operation. The compiler rejects a `match` that misses a variant.
- `Box<Expr>` is needed because a value of a recursive enum would otherwise have no fixed size.
- The `?` operator returns the error of `Var` from `evaluate` at once, so `Add` and `Mul` need no explicit error checks.
- `Display` makes the second operation, so `println!` shows an expression. The small `Wrap` type adds parentheses around a sum inside a product.

```rust run id=expr
enum Expr {
    Num(i64),
    Add(Box<Expr>, Box<Expr>),
    Mul(Box<Expr>, Box<Expr>),
}

use Expr::*;

// Each operation is one function with a match. The compiler checks that each variant has an arm.
fn show(e: &Expr) -> String {
    match e {
        Num(v) => v.to_string(),
        Add(l, r) => format!("({} + {})", show(l), show(r)),
        Mul(l, r) => format!("({} * {})", show(l), show(r)),
    }
}

fn evaluate(e: &Expr) -> i64 {
    match e {
        Num(v) => *v,
        Add(l, r) => evaluate(l) + evaluate(r),
        Mul(l, r) => evaluate(l) * evaluate(r),
    }
}

fn count(e: &Expr) -> usize {
    match e {
        Num(_) => 1,
        Add(l, r) | Mul(l, r) => 1 + count(l) + count(r),
    }
}

fn main() {
    let tree = Mul(Box::new(Add(Box::new(Num(2)), Box::new(Num(3)))), Box::new(Num(4)));
    println!("show: {}", show(&tree));
    println!("evaluate: {}", evaluate(&tree));
    println!("nodes: {}", count(&tree));
}
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

- Verdict: native. An enum is a sum type, and a `match` must have an arm for each variant. A variant added to `Expr` makes each of these functions a compile error until it gets an arm.
- `Box` gives a recursive enum a known size. Without it, an `Expr` would contain itself.
- The or-pattern `Add(l, r) | Mul(l, r)` shares one arm, because both variants bind the same names with the same types.
- A `Visitor` trait with a default method for each variant is the classic form in Rust. It fits a tree with many variants, where most operations care about a few of them and want a default for the rest.

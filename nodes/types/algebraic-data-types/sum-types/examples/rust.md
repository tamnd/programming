```rust run id=shapes
use std::f64::consts::PI;

enum Shape {
    Circle { radius: f64 },
    Rect { width: f64, height: f64 },
    Triangle { base: f64, height: f64 },
}

fn area(shape: &Shape) -> f64 {
    match shape {
        Shape::Circle { radius } => PI * radius * radius,
        Shape::Rect { width, height } => width * height,
        Shape::Triangle { base, height } => base * height / 2.0,
    }
}

fn main() {
    let shapes = [
        Shape::Circle { radius: 1.0 },
        Shape::Rect { width: 2.0, height: 3.0 },
        Shape::Triangle { base: 4.0, height: 5.0 },
    ];
    for shape in &shapes {
        println!("{:.2}", area(shape));
    }
}
```

```output for=shapes
3.14
6.00
10.00
```

```rust run id=expr
enum Expr {
    Num(i64),
    Add(Box<Expr>, Box<Expr>),
    Mul(Box<Expr>, Box<Expr>),
}

fn eval(e: &Expr) -> i64 {
    match e {
        Expr::Num(n) => *n,
        Expr::Add(a, b) => eval(a) + eval(b),
        Expr::Mul(a, b) => eval(a) * eval(b),
    }
}

fn main() {
    use Expr::*;
    let e = Mul(Box::new(Add(Box::new(Num(2)), Box::new(Num(3)))), Box::new(Num(4)));
    println!("{}", eval(&e));

    let found: Option<&str> = ["ada", "grace"].into_iter().find(|n| n.starts_with('g'));
    match found {
        Some(name) => println!("found {name}"),
        None => println!("no match"),
    }
}
```

```output for=expr
20
found grace
```

- Verdict: native. An `enum` lists its variants, and each variant can hold its own fields.
- A `match` must cover every variant. A missing arm is error E0004 (see Pitfalls on the concept page).
- A recursive variant needs indirection, such as `Box<Expr>`, so that the enum has a known size.
- `Option<T>` and `Result<T, E>` in the standard library are enums.

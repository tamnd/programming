```rust run id=traits
// Each case is a type, and each operation is a trait.
struct Num(i64);
struct Add<L, R>(L, R);

trait Eval {
    fn eval(&self) -> i64;
}

impl Eval for Num {
    fn eval(&self) -> i64 { self.0 }
}

impl<L: Eval, R: Eval> Eval for Add<L, R> {
    fn eval(&self) -> i64 { self.0.eval() + self.1.eval() }
}

// A new operation: a new trait with an impl for each existing case.
trait Show {
    fn show(&self) -> String;
}

impl Show for Num {
    fn show(&self) -> String { self.0.to_string() }
}

impl<L: Show, R: Show> Show for Add<L, R> {
    fn show(&self) -> String { format!("({} + {})", self.0.show(), self.1.show()) }
}

// A new case: a new type with an impl of each operation.
struct Neg<E>(E);

impl<E: Eval> Eval for Neg<E> {
    fn eval(&self) -> i64 { -self.0.eval() }
}

impl<E: Show> Show for Neg<E> {
    fn show(&self) -> String { format!("-{}", self.0.show()) }
}

// An operation can also cover a type from the standard library.
impl Eval for i64 {
    fn eval(&self) -> i64 { *self }
}

impl Show for i64 {
    fn show(&self) -> String { self.to_string() }
}

fn report<E: Eval + Show>(e: &E) {
    println!("{} = {}", e.show(), e.eval());
}

fn main() {
    report(&Add(Num(1), Neg(Add(Num(2), Num(3)))));
    report(&Add(10_i64, Neg(4_i64)));
}
```

```output for=traits
(1 + -(2 + 3)) = -4
(10 + -4) = 6
```

```rust fails expect=error:E0004 id=new-variant
enum Expr {
    Num(i64),
    Add(Box<Expr>, Box<Expr>),
    Neg(Box<Expr>), // a new variant
}

fn eval(e: &Expr) -> i64 {
    match e {
        Expr::Num(n) => *n,
        Expr::Add(l, r) => eval(l) + eval(r),
    }
}

fn main() {
    let e = Expr::Add(Box::new(Expr::Num(1)), Box::new(Expr::Neg(Box::new(Expr::Num(5)))));
    println!("{}", eval(&e));
}
```

```rust fails expect=error:E0117 id=orphan-rule
use std::fmt;

// Display and Vec both come from the standard library.
impl fmt::Display for Vec<i64> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} items", self.len())
    }
}

fn main() {
    println!("{}", vec![1_i64, 2, 3]);
}
```

- Verdict: idiom. With one generic struct for each case and one trait for each operation, both kinds of extension need only new code: a new operation is a new trait with an impl for each case, and a new case is a new struct with an impl of each trait. A trait can also cover a type from another crate, as `Eval for i64` shows, so integers act as numbers in an expression. The compiler checks every combination statically, because the type of an expression records its whole shape.
- With an `enum` the cases are closed. The first failing block adds the variant `Neg`, and the compiler rejects every `match` that does not cover it, with error E0004. The second failing block shows the limit on the open side: the orphan rule allows an impl only if the trait or the type comes from the current crate, and `Display` and `Vec` both come from the standard library, so the compiler reports E0117. A local wrapper type, the newtype pattern, gets around it.

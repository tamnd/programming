```rust run id=big-step-interpreter
use std::collections::HashMap;

// Big-step semantics: each statement maps a state to its final state.
enum Expr {
    Num(i64),
    Var(&'static str),
    Bin(char, Box<Expr>, Box<Expr>),
}

enum Stmt {
    Set(&'static str, Expr),
    Seq(Vec<Stmt>),
    While(Expr, Box<Stmt>),
}

type State = HashMap<&'static str, i64>;

fn eval(e: &Expr, s: &State) -> i64 {
    match e {
        Expr::Num(n) => *n,
        Expr::Var(x) => s[x],
        Expr::Bin(op, a, b) => {
            let (x, y) = (eval(a, s), eval(b, s));
            match op {
                '+' => x + y,
                '-' => x - y,
                '*' => x * y,
                '%' => x % y,
                _ => (x != y) as i64,
            }
        }
    }
}

fn exec(st: &Stmt, mut s: State) -> State {
    match st {
        Stmt::Set(x, e) => {
            let v = eval(e, &s);
            s.insert(x, v);
            s
        }
        Stmt::Seq(list) => list.iter().fold(s, |s, st| exec(st, s)),
        Stmt::While(cond, body) => {
            while eval(cond, &s) != 0 {
                s = exec(body, s);
            }
            s
        }
    }
}

fn bin(op: char, a: Expr, b: Expr) -> Expr {
    Expr::Bin(op, Box::new(a), Box::new(b))
}

fn main() {
    use Expr::{Num, Var};
    // while b != 0 { t := b; b := a % b; a := t }
    let gcd = Stmt::While(
        bin('!', Var("b"), Num(0)),
        Box::new(Stmt::Seq(vec![
            Stmt::Set("t", Var("b")),
            Stmt::Set("b", bin('%', Var("a"), Var("b"))),
            Stmt::Set("a", Var("t")),
        ])),
    );
    for (a, b) in [(1071, 462), (17, 5), (12, 18)] {
        let end = exec(&gcd, State::from([("a", a), ("b", b)]));
        println!("gcd({a}, {b}) = {}", end["a"]);
    }
}
```

```output for=big-step-interpreter
gcd(1071, 462) = 21
gcd(17, 5) = 1
gcd(12, 18) = 6
```

- Verdict: absent. Rust has no complete formal semantics yet; the Rust Reference is written in English, and research projects such as RustBelt and the MiniRust model, with the Miri interpreter that detects undefined behavior, describe parts of it formally. The example is a big-step semantics for a small language: `exec` takes a statement and a state and returns the final state, with one `match` arm for each rule. The enums are the syntax, and the program runs Euclid's algorithm written in that syntax.

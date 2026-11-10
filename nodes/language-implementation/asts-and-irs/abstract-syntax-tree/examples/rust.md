```rust run id=parse-walk
use std::collections::HashMap;

#[derive(Debug)]
enum Expr {
    Num(i64),
    Var(String),
    Bin(char, Box<Expr>, Box<Expr>),
}

struct Parser<'a> {
    src: &'a [u8],
    pos: usize,
}

impl<'a> Parser<'a> {
    fn peek(&mut self) -> u8 {
        while self.pos < self.src.len() && self.src[self.pos] == b' ' {
            self.pos += 1;
        }
        if self.pos < self.src.len() { self.src[self.pos] } else { 0 }
    }

    fn expr(&mut self) -> Result<Expr, String> {
        let mut left = self.term()?;
        while matches!(self.peek(), b'+' | b'-') {
            let op = self.src[self.pos] as char;
            self.pos += 1;
            left = Expr::Bin(op, Box::new(left), Box::new(self.term()?));
        }
        Ok(left)
    }

    fn term(&mut self) -> Result<Expr, String> {
        let mut left = self.factor()?;
        while self.peek() == b'*' {
            self.pos += 1;
            left = Expr::Bin('*', Box::new(left), Box::new(self.factor()?));
        }
        Ok(left)
    }

    fn factor(&mut self) -> Result<Expr, String> {
        let c = self.peek();
        if c == b'(' {
            self.pos += 1;
            let inner = self.expr()?;
            if self.peek() != b')' {
                return Err(format!("expected ) at {}", self.pos));
            }
            self.pos += 1;
            return Ok(inner);
        }
        let start = self.pos;
        let text = |p: &Parser| std::str::from_utf8(&p.src[start..p.pos]).unwrap().to_string();
        if c.is_ascii_digit() {
            while self.pos < self.src.len() && self.src[self.pos].is_ascii_digit() {
                self.pos += 1;
            }
            return Ok(Expr::Num(text(self).parse().unwrap()));
        }
        if c.is_ascii_alphabetic() {
            while self.pos < self.src.len() && self.src[self.pos].is_ascii_alphabetic() {
                self.pos += 1;
            }
            return Ok(Expr::Var(text(self)));
        }
        Err(format!("unexpected {:?} at {}", c as char, self.pos))
    }
}

fn show(e: &Expr, depth: usize) {
    let pad = "  ".repeat(depth);
    match e {
        Expr::Num(v) => println!("{pad}Num {v}"),
        Expr::Var(name) => println!("{pad}Var {name}"),
        Expr::Bin(op, l, r) => {
            println!("{pad}Bin {op}");
            show(l, depth + 1);
            show(r, depth + 1);
        }
    }
}

fn count(e: &Expr) -> usize {
    match e {
        Expr::Bin(_, l, r) => 1 + count(l) + count(r),
        _ => 1,
    }
}

fn names<'a>(e: &'a Expr, out: &mut Vec<&'a str>) {
    match e {
        Expr::Num(_) => {}
        Expr::Var(name) => out.push(name),
        Expr::Bin(_, l, r) => {
            names(l, out);
            names(r, out);
        }
    }
}

fn eval(e: &Expr, env: &HashMap<&str, i64>) -> i64 {
    match e {
        Expr::Num(v) => *v,
        Expr::Var(name) => env[name.as_str()],
        Expr::Bin(op, l, r) => {
            let (l, r) = (eval(l, env), eval(r, env));
            match op {
                '+' => l + r,
                '-' => l - r,
                _ => l * r,
            }
        }
    }
}

fn main() {
    let tree = Parser { src: b"1 + 2 * (3 - x)", pos: 0 }.expr().unwrap();
    show(&tree, 0);
    println!("nodes: {}", count(&tree));
    let mut found = Vec::new();
    names(&tree, &mut found);
    println!("names: {found:?}");
    println!("value at x = 1: {}", eval(&tree, &HashMap::from([("x", 1)])));
    println!("{tree:?}");
}
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
names: ["x"]
value at x = 1: 5
Bin('+', Num(1), Bin('*', Num(2), Bin('-', Num(3), Var("x"))))
```

- Verdict: idiom. An `enum` lists the kinds of node, and `match` must cover all of them. A recursive variant holds its children in `Box`, because an `Expr` that contained an `Expr` directly would have no finite size.
- The parser returns `Result`, and `?` passes a syntax error up through the recursive calls.
- `#[derive(Debug)]` prints the whole tree. The `syn` crate parses Rust source into its own tree types, and procedural macros use it. It is a crate, not part of the standard library.

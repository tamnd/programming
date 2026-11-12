```rust run id=precedence-levels
// expr   = term { ("+" | "-") term } .
// term   = factor { ("*" | "/") factor } .
// factor = number | "(" expr ")" .
enum Expr {
    Num(i64),
    Bin(char, Box<Expr>, Box<Expr>),
}

struct Parser {
    toks: Vec<String>,
    pos: usize,
}

impl Parser {
    fn peek(&self) -> Option<&str> {
        self.toks.get(self.pos).map(|s| s.as_str())
    }

    fn next(&mut self) -> String {
        self.pos += 1;
        self.toks[self.pos - 1].clone()
    }

    fn expr(&mut self) -> Expr {
        let mut left = self.term();
        while let Some(op @ ("+" | "-")) = self.peek() {
            let op = op.chars().next().unwrap();
            self.pos += 1;
            left = Expr::Bin(op, Box::new(left), Box::new(self.term())); // the loop groups to the left
        }
        left
    }

    fn term(&mut self) -> Expr {
        let mut left = self.factor();
        while let Some(op @ ("*" | "/")) = self.peek() {
            let op = op.chars().next().unwrap();
            self.pos += 1;
            left = Expr::Bin(op, Box::new(left), Box::new(self.factor()));
        }
        left
    }

    fn factor(&mut self) -> Expr {
        let t = self.next();
        if t == "(" {
            let e = self.expr();
            self.next(); // the closing parenthesis
            e
        } else {
            Expr::Num(t.parse().unwrap())
        }
    }
}

fn tokens(src: &str) -> Vec<String> {
    let mut out = Vec::new();
    let mut num = String::new();
    for c in src.chars() {
        if c.is_ascii_digit() {
            num.push(c);
            continue;
        }
        if !num.is_empty() {
            out.push(std::mem::take(&mut num));
        }
        if !c.is_whitespace() {
            out.push(c.to_string());
        }
    }
    if !num.is_empty() {
        out.push(num);
    }
    out
}

fn show(e: &Expr) -> String {
    match e {
        Expr::Num(n) => n.to_string(),
        Expr::Bin(op, a, b) => format!("({} {} {})", show(a), op, show(b)),
    }
}

fn eval(e: &Expr) -> i64 {
    match e {
        Expr::Num(n) => *n,
        Expr::Bin(op, a, b) => {
            let (x, y) = (eval(a), eval(b));
            match op {
                '+' => x + y,
                '-' => x - y,
                '*' => x * y,
                _ => x / y,
            }
        }
    }
}

fn main() {
    for src in ["10 - 3 - 2", "1 + 2 * 3", "(1 + 2) * 3", "100 / 10 / 5"] {
        let mut p = Parser { toks: tokens(src), pos: 0 };
        let e = p.expr();
        println!("{:<13} {:<20} = {}", src, show(&e), eval(&e));
    }
}
```

```output for=precedence-levels
10 - 3 - 2    ((10 - 3) - 2)       = 5
1 + 2 * 3     (1 + (2 * 3))        = 7
(1 + 2) * 3   ((1 + 2) * 3)        = 9
100 / 10 / 5  ((100 / 10) / 5)     = 2
```

- Verdict: library. pest uses PEG grammars, LALRPOP generates LR parsers, and nom and chumsky build parsers from combinators. The example turns a layered EBNF grammar into one function per rule. Each level of the grammar is one level of precedence, so `*` binds tighter than `+`, and each repetition in braces becomes a `while` loop that builds the tree from the left, so `10 - 3 - 2` is `(10 - 3) - 2`. The printed trees show both facts.

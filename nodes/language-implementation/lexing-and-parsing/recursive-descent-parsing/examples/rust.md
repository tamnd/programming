```rust run id=parser
#[derive(Clone)]
struct Token {
    kind: String,
    text: String,
    col: usize,
}

type Parsed = Result<(String, i64), String>;

fn tokenize(src: &str) -> Result<Vec<Token>, String> {
    let bytes = src.as_bytes();
    let mut tokens = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        let c = bytes[i] as char;
        if c == ' ' {
            i += 1;
        } else if c.is_ascii_digit() {
            let start = i;
            while i < bytes.len() && bytes[i].is_ascii_digit() {
                i += 1;
            }
            tokens.push(Token { kind: "num".into(), text: src[start..i].into(), col: start });
        } else if "+-*/^()".contains(c) {
            tokens.push(Token { kind: c.to_string(), text: c.to_string(), col: i });
            i += 1;
        } else {
            return Err(format!("unexpected character '{}' at column {}", c, i + 1));
        }
    }
    tokens.push(Token { kind: "end".into(), text: String::new(), col: src.len() });
    Ok(tokens)
}

fn combine(op: &str, (lt, a): (String, i64), (rt, b): (String, i64)) -> (String, i64) {
    let value = match op {
        "+" => a + b,
        "-" => a - b,
        "*" => a * b,
        "/" => a / b,
        _ => a.pow(b as u32),
    };
    (format!("({} {} {})", lt, op, rt), value)
}

struct Parser {
    tokens: Vec<Token>,
    i: usize,
}

impl Parser {
    fn peek(&self) -> &str {
        &self.tokens[self.i].kind
    }

    fn take(&mut self) -> Token {
        self.i += 1;
        self.tokens[self.i - 1].clone()
    }

    fn fail<T>(&self, expected: &str) -> Result<T, String> {
        Err(format!("expected {} at column {}", expected, self.tokens[self.i].col + 1))
    }

    fn expr(&mut self) -> Parsed {
        let mut left = self.term()?;
        while matches!(self.peek(), "+" | "-") {
            let op = self.take().kind;
            left = combine(&op, left, self.term()?);
        }
        Ok(left)
    }

    fn term(&mut self) -> Parsed {
        let mut left = self.unary()?;
        while matches!(self.peek(), "*" | "/") {
            let op = self.take().kind;
            left = combine(&op, left, self.unary()?);
        }
        Ok(left)
    }

    fn unary(&mut self) -> Parsed {
        if self.peek() != "-" {
            return self.power();
        }
        self.take();
        let (text, value) = self.unary()?;
        Ok((format!("(-{})", text), -value))
    }

    fn power(&mut self) -> Parsed {
        let base = self.atom()?;
        if self.peek() != "^" {
            return Ok(base);
        }
        self.take();
        Ok(combine("^", base, self.unary()?))
    }

    fn atom(&mut self) -> Parsed {
        match self.peek() {
            "num" => {
                let tok = self.take();
                Ok((tok.text.clone(), tok.text.parse().unwrap()))
            }
            "(" => {
                self.take();
                let inner = self.expr()?;
                if self.peek() != ")" {
                    return self.fail("')'");
                }
                self.take();
                Ok(inner)
            }
            _ => self.fail("a number or '('"),
        }
    }
}

fn parse(src: &str) -> Parsed {
    let mut p = Parser { tokens: tokenize(src)?, i: 0 };
    let result = p.expr()?;
    if p.peek() != "end" {
        return p.fail("an operator");
    }
    Ok(result)
}

fn main() {
    for src in ["1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"] {
        match parse(src) {
            Ok((text, value)) => println!("{} -> {} = {}", src, text, value),
            Err(msg) => println!("{} -> error: {}", src, msg),
        }
    }
}
```

```output for=parser
1 + 2 * 3 -> (1 + (2 * 3)) = 7
1 - 2 - 3 -> ((1 - 2) - 3) = -4
2 ^ 3 ^ 2 -> (2 ^ (3 ^ 2)) = 512
-2 ^ 2 -> (-(2 ^ 2)) = -4
(1 + 2) * 3 -> ((1 + 2) * 3) = 9
1 + * 2 -> error: expected a number or '(' at column 5
(1 + 2 -> error: expected ')' at column 7
1 $ 2 -> error: unexpected character '$' at column 3
```

- Verdict: idiom. Each rule is a method that returns `Result`. The `?` operator returns an error to the caller at once, so an error deep in the recursion travels up through every rule with no extra code.
- `fail` is generic in its success type. The same function serves a rule that returns a pair and the entry point.
- `matches!(self.peek(), "+" | "-")` tests the next token against a pattern with two choices and gives a `bool`.

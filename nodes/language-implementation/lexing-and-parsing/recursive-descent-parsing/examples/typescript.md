```typescript run id=parser
type Token = { kind: string; text: string; col: number };
type Result = [text: string, value: number];

class ParseError extends Error {}

function tokenize(src: string): Token[] {
  const tokens: Token[] = [];
  let i = 0;
  while (i < src.length) {
    const c = src[i];
    if (c === " ") {
      i++;
    } else if (c >= "0" && c <= "9") {
      let j = i;
      while (j < src.length && src[j] >= "0" && src[j] <= "9") j++;
      tokens.push({ kind: "num", text: src.slice(i, j), col: i });
      i = j;
    } else if ("+-*/^()".includes(c)) {
      tokens.push({ kind: c, text: c, col: i });
      i++;
    } else {
      throw new ParseError(`unexpected character '${c}' at column ${i + 1}`);
    }
  }
  tokens.push({ kind: "end", text: "", col: src.length });
  return tokens;
}

function combine(op: string, [lt, a]: Result, [rt, b]: Result): Result {
  let value: number;
  switch (op) {
    case "+": value = a + b; break;
    case "-": value = a - b; break;
    case "*": value = a * b; break;
    case "/": value = Math.trunc(a / b); break;
    default: value = a ** b;
  }
  return [`(${lt} ${op} ${rt})`, value];
}

class Parser {
  private tokens: Token[];
  private i = 0;

  constructor(src: string) {
    this.tokens = tokenize(src);
  }

  parse(): Result {
    const result = this.expr();
    if (this.peek() !== "end") this.fail("an operator");
    return result;
  }

  private peek(): string {
    return this.tokens[this.i].kind;
  }

  private take(): Token {
    return this.tokens[this.i++];
  }

  private fail(expected: string): never {
    throw new ParseError(`expected ${expected} at column ${this.tokens[this.i].col + 1}`);
  }

  private expr(): Result {
    let left = this.term();
    while (this.peek() === "+" || this.peek() === "-") left = combine(this.take().kind, left, this.term());
    return left;
  }

  private term(): Result {
    let left = this.unary();
    while (this.peek() === "*" || this.peek() === "/") left = combine(this.take().kind, left, this.unary());
    return left;
  }

  private unary(): Result {
    if (this.peek() !== "-") return this.power();
    this.take();
    const [text, value] = this.unary();
    return [`(-${text})`, -value];
  }

  private power(): Result {
    const base = this.atom();
    if (this.peek() !== "^") return base;
    this.take();
    return combine("^", base, this.unary());
  }

  private atom(): Result {
    const tok = this.tokens[this.i];
    if (tok.kind === "num") {
      this.take();
      return [tok.text, Number(tok.text)];
    }
    if (tok.kind === "(") {
      this.take();
      const inner = this.expr();
      if (this.peek() !== ")") this.fail("')'");
      this.take();
      return inner;
    }
    return this.fail("a number or '('");
  }
}

for (const src of ["1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"]) {
  try {
    const [text, value] = new Parser(src).parse();
    console.log(`${src} -> ${text} = ${value}`);
  } catch (e) {
    if (!(e instanceof ParseError)) throw e;
    console.log(`${src} -> error: ${e.message}`);
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

- Verdict: idiom. The parser is a class with one private method per rule. The result type is a labelled tuple of the text and the value.
- `fail` has the return type `never`. The checker knows that the code after a call to `fail` does not run, so `atom` can end with `return this.fail(...)`.
- A dedicated `ParseError` class separates errors in the input from bugs in the parser.

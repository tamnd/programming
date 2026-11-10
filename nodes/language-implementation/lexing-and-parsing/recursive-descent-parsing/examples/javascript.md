```javascript run id=parser
function tokenize(src) {
  const tokens = [];
  const re = /\s*(?:(\d+)|([-+*/^()])|(\S))/y;
  let m;
  while ((m = re.exec(src)) !== null) {
    if (m[3]) throw new SyntaxError(`unexpected character '${m[3]}' at column ${re.lastIndex}`);
    if (m[1]) tokens.push({ kind: "num", text: m[1], col: re.lastIndex - m[1].length });
    else if (m[2]) tokens.push({ kind: m[2], text: m[2], col: re.lastIndex - 1 });
    else break;
  }
  tokens.push({ kind: "end", text: "", col: src.length });
  return tokens;
}

function combine(op, [lt, a], [rt, b]) {
  const value = { "+": a + b, "-": a - b, "*": a * b, "/": Math.trunc(a / b), "^": a ** b }[op];
  return [`(${lt} ${op} ${rt})`, value];
}

function parse(src) {
  const tokens = tokenize(src);
  let i = 0;
  const peek = () => tokens[i].kind;
  const take = () => tokens[i++];
  const fail = (expected) => {
    throw new SyntaxError(`expected ${expected} at column ${tokens[i].col + 1}`);
  };

  function expr() {
    let left = term();
    while (peek() === "+" || peek() === "-") left = combine(take().kind, left, term());
    return left;
  }

  function term() {
    let left = unary();
    while (peek() === "*" || peek() === "/") left = combine(take().kind, left, unary());
    return left;
  }

  function unary() {
    if (peek() !== "-") return power();
    take();
    const [text, value] = unary();
    return [`(-${text})`, -value];
  }

  function power() {
    const base = atom();
    if (peek() !== "^") return base;
    take();
    return combine("^", base, unary());
  }

  function atom() {
    const tok = tokens[i];
    if (tok.kind === "num") {
      take();
      return [tok.text, Number(tok.text)];
    }
    if (tok.kind === "(") {
      take();
      const inner = expr();
      if (peek() !== ")") fail("')'");
      take();
      return inner;
    }
    fail("a number or '('");
  }

  const result = expr();
  if (peek() !== "end") fail("an operator");
  return result;
}

for (const src of ["1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"]) {
  try {
    const [text, value] = parse(src);
    console.log(`${src} -> ${text} = ${value}`);
  } catch (e) {
    if (!(e instanceof SyntaxError)) throw e;
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

- Verdict: idiom. The rule functions are closures inside `parse`, so they share the token list and the index without a class.
- The tokenizer uses a sticky regular expression (flag `y`). Each match must start where the last one ended, so no character is skipped without a check.
- `catch` tests `instanceof SyntaxError` and throws other errors again. A bug in the parser then shows as a bug, not as a syntax error of the input.

JavaScript itself refuses the expression that the grammar above accepts. A unary operator directly before the base of `**` is a syntax error, so `-2 ** 2` does not parse, and the programmer must write `(-2) ** 2` or `-(2 ** 2)`.

```javascript run id=js-power
for (const src of ["-2 ** 2", "(-2) ** 2", "-(2 ** 2)", "2 ** 3 ** 2"]) {
  try {
    console.log(`${src} = ${new Function(`return ${src};`)()}`);
  } catch (e) {
    console.log(`${src} -> ${e.name}`);
  }
}
```

```output for=js-power
-2 ** 2 -> SyntaxError
(-2) ** 2 = 4
-(2 ** 2) = -4
2 ** 3 ** 2 = 512
```

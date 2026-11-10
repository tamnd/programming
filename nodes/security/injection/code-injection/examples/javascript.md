```javascript wrong id=eval
function calculate(expression) {
  // The feature: a calculator for the formulas that users type.
  return eval(expression);
}

console.log(calculate("6 * 7"));
console.log(calculate("process.release.name"));
```

```output for=eval
42
node
```

The second call reads the global `process` object, which a calculator never offered. The same input could reach the file system or start a process.

```javascript run id=parser
// A parser for numbers, + - * / and parentheses. Anything else is an error.
function calculate(text) {
  if (text.length > 100) throw new Error("expression too long");
  const tokens = text.match(/\d+(?:\.\d+)?|[-+*/()]|\S/g) ?? [];
  let pos = 0;

  function primary() {
    const token = tokens[pos++];
    if (token === "(") {
      const value = sum();
      if (tokens[pos++] !== ")") throw new Error("missing )");
      return value;
    }
    if (token === "-") return -primary();
    if (/^\d/.test(token ?? "")) return Number(token);
    throw new Error(`unexpected ${JSON.stringify(token ?? "end")}`);
  }

  function product() {
    let value = primary();
    while (tokens[pos] === "*" || tokens[pos] === "/") {
      value = tokens[pos++] === "*" ? value * primary() : value / primary();
    }
    return value;
  }

  function sum() {
    let value = product();
    while (tokens[pos] === "+" || tokens[pos] === "-") {
      value = tokens[pos++] === "+" ? value + product() : value - product();
    }
    return value;
  }

  const value = sum();
  if (pos !== tokens.length) throw new Error(`unexpected ${JSON.stringify(tokens[pos])}`);
  return value;
}

for (const text of ["6 * 7", "(1 + 2) / 4", "process.release.name"]) {
  try {
    console.log(text, "=", calculate(text));
  } catch (error) {
    console.log(text, "->", error.message);
  }
}
```

```output for=parser
6 * 7 = 42
(1 + 2) / 4 = 0.75
process.release.name -> unexpected "p"
```

The parser knows numbers, four operators and parentheses. A name is not in its grammar, so it stops at the first letter.

- Verdict: stdlib. JavaScript has no safe form of `eval`. A small recursive descent parser, as above, covers formulas. `JSON.parse` covers data.
- `eval`, `new Function(text)`, and `setTimeout` or `setInterval` with a string all compile text as code. MDN says to never use direct `eval`, because it runs with the privileges of the caller and can read the local variables.
- The `node:vm` module is not a security mechanism. Its documentation says not to use it to run untrusted code.
- To call a function by a name from input, use a `Map` from names to functions. Do not index `globalThis` or an object with the input, because the name can reach methods that you did not mean to offer.
- In the browser, the same weakness with HTML is [[cross-site-scripting]]. A Content Security Policy without `'unsafe-eval'` makes the browser refuse `eval` and `new Function`.
- ESLint rules `no-eval`, `no-implied-eval` and `no-new-func` flag these calls. CodeQL has `js/code-injection` and `js/unsafe-dynamic-method-access`.

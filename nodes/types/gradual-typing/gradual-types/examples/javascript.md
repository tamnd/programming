```javascript run id=contracts-with-blame
// A contract wraps a function and checks its arguments and result at the
// boundary between checked and unchecked code, and names the party to blame.
function contract(fn, params, result, name) {
  return (...args) => {
    params.forEach((type, i) => {
      if (typeof args[i] !== type) {
        throw new TypeError(`${name}: caller broke the contract: argument ${i} is ${typeof args[i]}, not ${type}`);
      }
    });
    const out = fn(...args);
    if (typeof out !== result) {
      throw new TypeError(`${name}: function broke the contract: result is ${typeof out}, not ${result}`);
    }
    return out;
  };
}

const add = contract((a, b) => a + b, ["number", "number"], "number", "add");
const label = contract((n) => (n > 0 ? "positive" : 0), ["number"], "string", "label");

console.log("unchecked:", ((a, b) => a + b)("1", 2)); // plain JavaScript joins the strings
for (const call of [() => add(1, 2), () => add("1", 2), () => label(5), () => label(-5)]) {
  try {
    console.log(call());
  } catch (err) {
    console.log(err.message);
  }
}
```

```output for=contracts-with-blame
unchecked: 12
3
add: caller broke the contract: argument 0 is string, not number
positive
label: function broke the contract: result is number, not string
```

- Verdict: absent. JavaScript has no type annotations; TypeScript and Flow add them as separate languages that compile to JavaScript and remove the types. The example writes a contract in the style of Racket: a wrapper checks the arguments and the result of a function at the boundary and reports which side broke the agreement. Bad arguments blame the caller, and a bad result blames the function, which is the blame that gradual type systems assign to casts.

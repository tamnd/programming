```javascript run id=tdz-in-function
function show() {
  return value;
}

try {
  show();
} catch (e) {
  console.log(e.name);
}
let value = 3;
console.log(show());

let later;
console.log(later === undefined);
```

```output for=tdz-in-function
ReferenceError
3
true
```

- Verdict: native. The rule is about time, not about the place in the text: `show` reads `value`, and the first call fails because the line `let value = 3` has not run yet. After that line, the same call works. `let later;` has no initializer, so it holds `undefined`, with no error.

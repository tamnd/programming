```javascript run id=tdz
console.log(typeof early, early);
var early = 1;

try {
  console.log(late);
} catch (e) {
  console.log(e.name);
}
let late = 2;

let empty;
console.log(late, empty);
```

```output for=tdz
undefined undefined
ReferenceError
2 undefined
```

- Verdict: native. A `var` variable exists from the start of the function with the value `undefined`, so the first line reads it with no error. A `let` variable is in the temporal dead zone until its line runs, and a read there throws a `ReferenceError`. `let empty;` with no initializer gives `undefined`.

```javascript run id=let-and-var-counters
// Each pass of a let loop has its own i, so each closure sees its own value.
const withLet = [];
for (let i = 0; i < 3; i++) withLet.push(() => i);
console.log("let:", withLet.map((f) => f()).join(" "));

// A var counter is one variable for the whole function.
const withVar = [];
for (var j = 0; j < 3; j++) withVar.push(() => j);
console.log("var:", withVar.map((f) => f()).join(" "));
console.log("j after the loop:", j);

// Two counters in one header walk from both ends.
const letters = ["a", "b", "c", "d", "e"];
for (let lo = 0, hi = letters.length - 1; lo < hi; lo++, hi--) {
  [letters[lo], letters[hi]] = [letters[hi], letters[lo]];
}
console.log(letters.join(""));

// All three parts can be empty.
let n = 0;
for (;;) {
  if (++n === 3) break;
}
console.log("n:", n);
```

```output for=let-and-var-counters
let: 0 1 2
var: 3 3 3
j after the loop: 3
edcba
n: 3
```

- Verdict: native. With `let`, each pass has a new binding, so the three closures return 0, 1 and 2. With `var`, the closures share one variable, which is 3 after the loop and still exists there. Two counters in one header reverse the array in place. `for (;;)` has no condition and stops only at `break`.

```javascript run id=const-steps
const prices = [3, 4, 5];
const total = prices.reduce((sum, p) => sum + p, 0);
const label = total > 10 ? "big" : "small";
console.log(total, label);

const handlers = [];
for (let i = 0; i < 3; i++) {
  handlers.push(() => i);
}
console.log(handlers.map((h) => h()));
```

```output for=const-steps
12 big
[ 0, 1, 2 ]
```

- Verdict: native. `const` names get one value. `reduce` replaces a loop that changes a total, and the conditional operator replaces an `if` that assigns `label` in two places. A `let` in a `for` loop makes a new variable on each pass, so each function keeps its own `i`.

```javascript run id=ties
const halfAway = (x) => Math.sign(x) * Math.round(Math.abs(x));
const halfEven = (x) => {
  const r = Math.round(x);
  return Math.abs(x % 1) === 0.5 && r % 2 !== 0 ? r - 1 : r;
};

const pad = (v) => String(v + 0).padStart(5);
console.log("    x round  away  even floor  ceil trunc");
for (const x of [0.5, 1.5, 2.5, -0.5, -1.5, -2.5]) {
  const cells = [Math.round(x), halfAway(x), halfEven(x), Math.floor(x), Math.ceil(x), Math.trunc(x)];
  console.log(pad(x) + " " + cells.map(pad).join(" "));
}

console.log("Object.is(Math.round(-0.5), -0):", Object.is(Math.round(-0.5), -0));
console.log("(2.5).toFixed(0):", (2.5).toFixed(0), "(-2.5).toFixed(0):", (-2.5).toFixed(0));
```

```output for=ties
    x round  away  even floor  ceil trunc
  0.5     1     1     0     0     1     0
  1.5     2     2     2     1     2     1
  2.5     3     3     2     2     3     2
 -0.5     0    -1     0    -1     0     0
 -1.5    -1    -2    -2    -2    -1    -1
 -2.5    -2    -3    -2    -3    -2    -2
Object.is(Math.round(-0.5), -0): true
(2.5).toFixed(0): 3 (-2.5).toFixed(0): -3
```

- Verdict: native. `Math.round`, `Math.floor`, `Math.ceil` and `Math.trunc` are built in. JavaScript has no built-in half to even or half away from zero, so the code writes them.
- `Math.round` sends a tie toward plus infinity: 2.5 becomes 3 but -2.5 becomes -2. `Math.round(-0.5)` is negative zero, and the table adds 0 to print it as 0.
- `toFixed` uses another rule than `Math.round`: in this run it rounds -2.5 to -3.

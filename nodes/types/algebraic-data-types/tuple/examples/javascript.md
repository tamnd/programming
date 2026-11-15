```javascript run id=pair
const yn = (b) => (b ? "yes" : "no");

function minMax(values) {
  return [Math.min(...values), Math.max(...values)];
}

const pair = minMax([7, 2, 9, 4]);
console.log("pair:", JSON.stringify(pair));
const [lo, hi] = pair;
console.log("lo:", lo, "hi:", hi);
console.log("second part:", pair[1]);

const other = [2, 9];
const samePair = (a, b) => a.length === b.length && a.every((x, i) => x === b[i]);
console.log("equal pairs:", yn(samePair(pair, other)));
console.log("equal pairs, ===:", yn(pair === other));
```

```output for=pair
pair: [2,9]
lo: 2 hi: 9
second part: 9
equal pairs: yes
equal pairs, ===: no
```

- Verdict: idiom. JavaScript has no tuple type. A short array plays the role, and destructuring unpacks it.
- Nothing fixes the length or the types of the parts.
- `===` compares identity, so two arrays with the same parts are not `===`.

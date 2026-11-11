```javascript run id=points
function describe([x, y]) {
  if (x === 0 && y === 0) return "origin";
  if (y === 0) return `on the x axis at ${x}`;
  if (x === 0) return `on the y axis at ${y}`;
  if (x === y) return `on the diagonal at ${x}`;
  return `at ${x}, ${y}`;
}

const points = [[0, 0], [3, 0], [0, -2], [4, 4], [1, 5]];
for (const point of points) {
  console.log(`(${point.join(", ")}): ${describe(point)}`);
}

const { type, pos: [px, py] = [0, 0], size } = { type: "click", pos: [10, 20] };
console.log(type, px, py, size);
```

```output for=points
(0, 0): origin
(3, 0): on the x axis at 3
(0, -2): on the y axis at -2
(4, 4): on the diagonal at 4
(1, 5): at 1, 5
click 10 20 undefined
```

- Verdict: idiom. JavaScript has destructuring but no `match`. Destructuring binds names, but it does not test the shape of the value.
- A missing property gives `undefined`, or the default value after `=`. Here `size` is `undefined`, and nothing reports it.
- The TC39 pattern matching proposal is at Stage 1. It would add a `match` expression and an `is` operator.

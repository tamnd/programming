```javascript run id=labels
const grid = [
  [3, 1],
  [4, -1],
  [-5, 9],
];

let found = null;
search: for (const [r, row] of grid.entries()) {
  for (const [c, value] of row.entries()) {
    if (value < 0) {
      found = [r, c];
      break search;
    }
  }
}
console.log(found);

outer: for (let i = 0; i < 3; i++) {
  for (let j = 0; j < 3; j++) {
    if (j > i) continue outer;
    if (i === 2) break outer;
    console.log(i, j);
  }
}
```

```output for=labels
[ 1, 1 ]
0 0
1 0
1 1
```

- Verdict: native. JavaScript has no `goto`. A label on a loop or a block lets `break` and `continue` leave several levels of nesting, but only outward, and only to an enclosing statement.

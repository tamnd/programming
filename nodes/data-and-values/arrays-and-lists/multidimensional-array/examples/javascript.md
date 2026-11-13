```javascript run id=array-from-rows
// An array of arrays. Array.from calls the function once per row, so each row is new.
const grid = Array.from({ length: 2 }, () => Array(3).fill(0));
grid[0][1] = 5;
console.log(grid, grid.length, grid[0].length);

// fill puts the same row object in every slot.
const bad = Array(2).fill(Array(3).fill(0));
bad[0][1] = 5;
console.log(bad, bad[0] === bad[1]);

// Rows can have different lengths.
const jagged = [[1], [2, 3], [4, 5, 6]];
console.log(jagged.map((row) => row.length), jagged[2][1], jagged[5]?.[0]);

// A typed array with an index formula is a dense matrix in row-major order.
const rows = 2, cols = 3;
const m = new Float64Array(rows * cols);
for (let r = 0; r < rows; r++) for (let c = 0; c < cols; c++) m[r * cols + c] = 10 * r + c;
console.log(Array.from(m), m[1 * cols + 2]);

// The transpose swaps rows and columns.
const t = [[1, 2, 3], [4, 5, 6]];
console.log(t[0].map((_, c) => t.map((row) => row[c])));
```

```output
[ [ 0, 5, 0 ], [ 0, 0, 0 ] ] 2 3
[ [ 0, 5, 0 ], [ 0, 5, 0 ] ] true
[ 1, 2, 3 ] 5 undefined
[ 0, 1, 2, 10, 11, 12 ] 12
[ [ 1, 4 ], [ 2, 5 ], [ 3, 6 ] ]
```

- Verdict: idiom. Array.from calls its function once per row, so each row is a new array. fill puts the same row object in every slot. Rows can differ in length, and ?. reads a missing row as undefined. A Float64Array with an index formula is a dense matrix in row-major order. A map over the columns gives the transpose.

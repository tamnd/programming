```javascript run id=map-filter-reduce-javascript
const nums = [1, 2, 3, 4, 5, 6];
const squares = nums.filter((x) => x % 2 === 0).map((x) => x * x);
console.log(squares.join(" "));
console.log(squares.reduce((acc, x) => acc + x, 0));
```

```output for=map-filter-reduce-javascript
4 16 36
56
```

- Verdict: native. `filter`, `map` and `reduce` are methods of every array. Each call to `filter` or `map` makes a new array.

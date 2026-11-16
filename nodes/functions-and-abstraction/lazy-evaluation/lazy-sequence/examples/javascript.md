```javascript run id=pipeline
function* naturals() {
  for (let n = 1; ; n++) yield n;
}

const oddSquares = naturals()
  .map((n) => {
    console.log("square", n);
    return n * n;
  })
  .filter((s) => s % 2 === 1);

console.log(oddSquares.take(2).toArray());
```

```output for=pipeline
square 1
square 2
square 3
[ 1, 9 ]
```

- Verdict: native. A generator object is an iterator, and the iterator helpers `map`, `filter`, `take`, `drop` and `flatMap` on it return lazy iterators. `toArray` is a terminal operation that pulls the elements.
- Array methods such as `Array.prototype.map` are eager: they build a full array at each step.

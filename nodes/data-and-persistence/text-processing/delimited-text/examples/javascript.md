```javascript run id=split-fields
// split with a string keeps every empty field, also the last one.
console.log(JSON.stringify("a::b:".split(":")));
console.log(JSON.stringify("".split(":")));

// For fields split by white space, trim first and split at runs of it.
console.log(JSON.stringify("  a  b ".trim().split(/\s+/)));

// A limit stops after that many fields and drops the rest of the line.
console.log(JSON.stringify("k=v=w".split("=", 2)));
```

```output for=split-fields
["a","","b",""]
[""]
["a","b"]
["k","v"]
```

- Verdict: stdlib. `String.prototype.split` with a string separator keeps each empty field, also the last one. Its `limit` argument stops after that many fields and drops the rest of the line, so a field that holds the separator needs `indexOf` and `slice` instead.

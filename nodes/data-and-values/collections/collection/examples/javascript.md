```javascript run id=three-kinds
const words = ["red", "blue", "red", "green"];
const unique = new Set(words);
const counts = new Map();
for (const w of words) {
  counts.set(w, (counts.get(w) ?? 0) + 1);
}

console.log(words.length, words[1], words.includes("red"));
console.log(unique.size, unique.has("red"), [...unique]);
console.log(counts.size, counts.get("red"), [...counts.keys()]);
```

```output for=three-kinds
4 blue true
3 true [ 'red', 'blue', 'green' ]
3 2 [ 'red', 'blue', 'green' ]
```

- Verdict: native. `Array`, `Set` and `Map` are built-in objects. An array has a literal syntax; a set and a map are made with `new`.
- A `Set` and a `Map` give their elements in the order of insertion, so the output order is fixed.
- The size of an array is `length`, and the size of a set or a map is `size`.

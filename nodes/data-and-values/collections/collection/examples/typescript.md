```typescript run id=three-kinds
const words: string[] = ["red", "blue", "red", "green"];
const unique: Set<string> = new Set(words);
const counts: Map<string, number> = new Map();
for (const w of words) {
  counts.set(w, (counts.get(w) ?? 0) + 1);
}

function size(c: { length: number } | { size: number }): number {
  return "length" in c ? c.length : c.size;
}

console.log(size(words), size(unique), size(counts));
console.log(words[1], unique.has("red"), counts.get("red"));
```

```output for=three-kinds
4 3 3
blue true 2
```

- Verdict: native. TypeScript uses the JavaScript collections and gives them item types, such as `string[]`, `Set<string>` and `Map<string, number>`.
- The checker rejects a call such as `counts.set("red", "two")`, because the values must be numbers.
- The function `size` shows a difference between the kinds: an array has `length`, and a set or a map has `size`.

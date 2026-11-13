```javascript run id=map-sort-map
let calls = 0;
const vowels = (w) => {
  calls++;
  return [...w].filter((c) => "aeiou".includes(c)).length;
};
const words = ["banana", "kiwi", "apple", "fig", "cherry", "date", "plum", "mango"];

// Without the technique: two key calls at each comparison.
calls = 0;
const slow = words.toSorted((a, b) => vowels(a) - vowels(b));
console.log(slow.join(" "), calls > words.length);

// Decorate with map, sort the pairs, undecorate with map.
calls = 0;
const fast = words
  .map((w) => [vowels(w), w])
  .sort((a, b) => a[0] - b[0])
  .map(([, w]) => w);
console.log(fast.join(" "), calls);
```

```output
fig cherry plum kiwi apple date mango banana true
fig cherry plum kiwi apple date mango banana 8
```

The compare function calls `vowels` twice at each comparison, so the key runs more than 8 times. `map` builds pairs of key and word, `sort` compares the stored keys, and a second `map` keeps the words, so the key runs 8 times. The JavaScript sort is stable, so equal keys keep their input order.

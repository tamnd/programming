```javascript run id=five-needs
const words = "the cat saw the dog and the cat ran".split(" ");

const seen = new Set(words);               // membership, no duplicates
const counts = new Map();                  // word to count
for (const w of words) counts.set(w, (counts.get(w) ?? 0) + 1);
const line = words.slice(0, 3);            // no deque type: an array as a queue
const first = line.shift();
const second = line.shift();

console.log(`list: ${words.length} words, third is ${words[2]}`);
console.log(`set: ${seen.size} distinct`);
console.log(`map: the=${counts.get("the")} cat=${counts.get("cat")}`);
console.log(`queue: ${first}, then ${second}`);
console.log("sorted:", [...counts.keys()].sort().join(" ")); // no sorted map: sort the keys
```

```output for=five-needs
list: 9 words, third is saw
set: 6 distinct
map: the=3 cat=2
queue: the, then cat
sorted: and cat dog ran saw the
```

- JavaScript has no deque type, so the queue is an array with `shift`. A `Map` keeps its keys in insertion order, so the keys must be sorted to print them in order.

```javascript run id=map-order
// A Map iterates its keys in insertion order.
const m = new Map();
m.set("pear", 4).set("apple", 7).set("fig", 2);
console.log([...m.keys()].join(" "));

// Setting a key that exists keeps its place. Delete and set moves it to the end.
m.set("pear", 9);
console.log([...m].join(" "));
m.delete("pear");
m.set("pear", 1);
console.log([...m.keys()].join(" "));

// The first key is the oldest. This makes a small LRU cache.
function touch(cache, k, v, size) {
  cache.delete(k);
  cache.set(k, v);
  if (cache.size > size) cache.delete(cache.keys().next().value);
}
const lru = new Map();
touch(lru, "a", 1, 2);
touch(lru, "b", 2, 2);
touch(lru, "a", 1, 2);
touch(lru, "c", 3, 2);
console.log([...lru.keys()].join(" "));

// A plain object is different: keys that look like array indexes come first,
// in number order, and then the other strings in insertion order.
const o = { b: 1, 10: 2, a: 3, 2: 4 };
console.log(Object.keys(o).join(" "));

// JSON.stringify writes the keys of an object in the same order.
console.log(JSON.stringify(Object.fromEntries(m)));
```

```output
pear apple fig
pear,9 apple,7 fig,2
apple fig pear
a c
2 10 b a
{"apple":7,"fig":2,"pear":1}
```

- Verdict: native. A Map iterates in insertion order. Setting an existing key keeps its place, and delete then set moves it to the end, which makes a small LRU cache. A plain object puts keys that look like array indexes first, in number order.

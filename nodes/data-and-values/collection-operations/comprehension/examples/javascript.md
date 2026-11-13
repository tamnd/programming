```javascript run id=map-filter-flatmap
const words = ["apple", "fig", "banana", "kiwi", "cherry"];

// JavaScript has no comprehension syntax. filter and map do the same work.
console.log(words.filter((w) => w.length > 3).map((w) => w.length));

// A Set from mapped values, and an object from [key, value] pairs.
console.log([...new Set(words.map((w) => w[0]))].sort());
console.log(Object.fromEntries(words.filter((w) => w.includes("i")).map((w) => [w, w.length])));

// flatMap does the work of two for clauses.
console.log([1, 2].flatMap((x) => ["a", "b"].map((y) => `${x}${y}`)));

// Array.from with a map function builds a list from a range.
console.log(Array.from({ length: 5 }, (_, i) => i * i));

// Iterator helpers are lazy: map runs only for the items that take needs.
let calls = 0;
const firstTwo = words
  .values()
  .map((w) => {
    calls++;
    return w.toUpperCase();
  })
  .take(2)
  .toArray();
console.log(firstTwo, calls);
```

```output
[ 5, 6, 4, 6 ]
[ 'a', 'b', 'c', 'f', 'k' ]
{ fig: 3, kiwi: 4 }
[ '1a', '1b', '2a', '2b' ]
[ 0, 1, 4, 9, 16 ]
[ 'APPLE', 'FIG' ] 2
```

- Verdict: idiom. JavaScript has no comprehension syntax. filter and map give the list, new Set gives a set, and Object.fromEntries makes an object from pairs. flatMap does the work of two for clauses. The iterator helpers on words.values() are lazy, so map runs only twice because take(2) needs only two items.

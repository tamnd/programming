```javascript run id=to-the-interface
function total(xs) {
  let sum = 0;
  for (const x of xs) sum += x; // any iterable
  return sum;
}

function describe(c) {
  const size = c.length ?? c.size; // no shared name for the size
  return `size ${size}, total ${total(c)}`;
}

const prices = new Map([["tea", 3], ["cake", 4]]);
console.log("list:", describe([1, 2, 3]));
console.log("set:", describe(new Set([4, 5])));
console.log("map values:", describe([...prices.values()]));
console.log(total(prices.values()), prices.values().size);
```

```output for=to-the-interface
list: size 3, total 6
set: size 2, total 9
map values: size 2, total 7
7 undefined
```

- Verdict: native. The iterable protocol is the one shared interface: an object with a `Symbol.iterator` method works with `for...of` and the spread syntax.
- JavaScript has no shared size interface. An array has `length`, a set and a map have `size`, and the iterator from `values()` has neither, so the code copies the values into an array before it calls `describe`.

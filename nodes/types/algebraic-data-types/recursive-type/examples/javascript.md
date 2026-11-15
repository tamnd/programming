```javascript run id=list
const cons = (head, tail) => ({ head, tail });

function total(xs) {
  return xs === null ? 0 : xs.head + total(xs.tail);
}

function length(xs) {
  return xs === null ? 0 : 1 + length(xs.tail);
}

function show(xs) {
  const parts = [];
  for (let n = xs; n !== null; n = n.tail) parts.push(n.head);
  return parts.join(" -> ");
}

const xs = cons(1, cons(2, cons(3, null)));
console.log("sum:", total(xs));
console.log("length:", length(xs));
console.log("list:", show(xs));
console.log("json:", JSON.stringify(xs));
```

```output for=list
sum: 6
length: 3
list: 1 -> 2 -> 3
json: {"head":1,"tail":{"head":2,"tail":{"head":3,"tail":null}}}
```

- Verdict: idiom. JavaScript has no type declarations, so any object property can hold another object of the same shape. `null` marks the end of the list.
- `JSON.stringify` walks the nested objects, so the output shows the recursive shape.

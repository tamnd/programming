```typescript run id=list
type IntList = { head: number; tail: IntList } | null;

type Json = string | number | boolean | null | Json[] | { [key: string]: Json };

function total(xs: IntList): number {
  return xs === null ? 0 : xs.head + total(xs.tail);
}

function length(xs: IntList): number {
  return xs === null ? 0 : 1 + length(xs.tail);
}

function show(xs: IntList): string {
  const parts: number[] = [];
  for (let n = xs; n !== null; n = n.tail) parts.push(n.head);
  return parts.join(" -> ");
}

function depth(j: Json): number {
  if (Array.isArray(j)) return 1 + Math.max(0, ...j.map(depth));
  if (j !== null && typeof j === "object") return 1 + Math.max(0, ...Object.values(j).map(depth));
  return 0;
}

const xs: IntList = { head: 1, tail: { head: 2, tail: { head: 3, tail: null } } };
console.log("sum:", total(xs));
console.log("length:", length(xs));
console.log("list:", show(xs));
const doc: Json = { name: "tree", children: [{ name: "leaf", children: [] }] };
console.log("json depth:", depth(doc));
```

```output for=list
sum: 6
length: 3
list: 1 -> 2 -> 3
json depth: 4
```

- Verdict: native. A type alias can refer to itself inside an object type, an array type or a union. `IntList` is an object with a tail of type `IntList`, or `null`.
- `Json` describes any JSON value: an array of `Json`, or an object whose values are `Json`.
- `depth` follows the shape of the `Json` type: one call for each array or object.

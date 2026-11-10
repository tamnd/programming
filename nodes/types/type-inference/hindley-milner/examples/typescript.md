```typescript run id=infer
let mode = "read";
const fixed = "read";
mode = "write";

const values = [0, 1, null];
const names = ["Ada", "Grace"];
const lengths = names.map((name) => name.length);

function half(n: number) {
  return n / 2;
}

console.log(mode, fixed, values.length, lengths.join(","), half(5));
console.log(typeof half(5), typeof fixed);
```

```output for=infer
write read 3 3,5 2.5
number string
```

- Verdict: native. `mode` has the type `string`, `fixed` has the literal type `"read"`, and `values` has the type `(number | null)[]`.
- The parameter `name` of the arrow function gets the type `string` from its context, the call of `map` on a `string[]`.
- `half` gets the return type `number` from its `return` statement.
- The runner removes the types and does not check them. The output shows only the values, which are the same as in JavaScript.

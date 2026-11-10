```javascript run id=tree
function total(item) {
  if (typeof item === "number") return item;
  let sum = 0;
  for (const child of item) sum += total(child);
  return sum;
}

function depth(n) {
  return n === 0 ? 0 : 1 + depth(n - 1);
}

console.log(total([1, [2, 3], [[4], 5]]));
console.log(depth(5000));
try {
  depth(10_000_000);
} catch (error) {
  console.log(error.name + ": " + error.message);
}
```

```output for=tree
15
5000
RangeError: Maximum call stack size exceeded
```

- Verdict: native. A function declaration can call itself by its name, and a named function expression can call itself by the name inside the expression.
- When the engine runs out of stack, it throws a `RangeError`. In V8, the message is "Maximum call stack size exceeded", and the program can catch the error.
- The specification requires proper tail calls in strict mode code, but V8 does not ship them. A tail call in Node.js still takes a frame (see Pitfalls in the node).
- The usual way around the depth limit is a loop with an explicit stack, or a trampoline.

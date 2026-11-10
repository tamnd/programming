```typescript run id=depth
interface Order {
  id: number;
  total: number;
}

function makeOrder(id: number): Order {
  const order: Order = { id, total: 0 };
  return order;
}

function countDown(n: number): number {
  return n === 0 ? 0 : 1 + countDown(n - 1);
}

const order = makeOrder(7);
const alias = order;
alias.total = 250;
console.log(order.id, order.total);

console.log(countDown(1000));
try {
  countDown(10_000_000);
} catch (error) {
  console.log(error instanceof RangeError);
}
```

```output
7 250
1000
true
```

- Verdict: native. TypeScript compiles to JavaScript, so the stack and the heap are those of the JavaScript engine. The types do not change where a value lives.
- `alias` and `order` refer to the same object on the heap, so a change through one name shows through the other.
- A depth of 1000 calls fits in the stack. A depth of ten million does not, and Node.js throws a `RangeError`.

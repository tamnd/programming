```typescript run id=two-layouts
interface ListNode<T> {
  value: T;
  next: ListNode<T> | null;
}

const array: number[] = [10, 20, 30, 40, 50];
let head: ListNode<number> | null = null;
for (const v of [...array].reverse()) {
  head = { value: v, next: head };
}

function get<T>(node: ListNode<T>, i: number): [T, number] {
  let steps = 0;
  for (; i > 0; i--, steps++) node = node.next!;
  return [node.value, steps];
}

console.log(`array[3] = ${array[3]} (0 steps)`);
const [value, steps] = get(head!, 3);
console.log(`linked[3] = ${value} (${steps} steps)`);

head = { value: 5, next: head };
const out: number[] = [];
for (let n: ListNode<number> | null = head; n; n = n.next) out.push(n.value);
console.log("after push_front:", out.join(" "));
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: native. TypeScript uses the JavaScript structures. An interface with a type parameter describes the node of a linked list.
- The type `ListNode<T> | null` makes the end of the list explicit, and the checker asks the code to handle it. The `!` tells the checker that the node is not null.

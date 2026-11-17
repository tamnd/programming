```javascript run id=two-layouts
const array = [10, 20, 30, 40, 50];
let head = null;
for (const v of [...array].reverse()) {
  head = { value: v, next: head };
}

function get(node, i) {
  let steps = 0;
  for (; i > 0; i--, steps++) node = node.next;
  return [node.value, steps];
}

console.log(`array[3] = ${array[3]} (0 steps)`);
const [value, steps] = get(head, 3);
console.log(`linked[3] = ${value} (${steps} steps)`);

head = { value: 5, next: head };
const out = [];
for (let n = head; n; n = n.next) out.push(n.value);
console.log("after push_front:", out.join(" "));
```

```output for=two-layouts
array[3] = 40 (0 steps)
linked[3] = 40 (3 steps)
after push_front: 5 10 20 30 40 50
```

- Verdict: native. `Array`, `Map` and `Set` are built in. A linked list is a chain of plain objects, each with a `value` and a `next` property.

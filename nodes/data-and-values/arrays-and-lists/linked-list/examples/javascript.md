```javascript run id=reverse-and-cycle
// A singly linked list as plain objects: { value, next }. null ends the list.
function fromArray(items) {
  let head = null;
  for (let i = items.length - 1; i >= 0; i--) {
    head = { value: items[i], next: head };
  }
  return head;
}

function toArray(head) {
  const out = [];
  for (let n = head; n !== null; n = n.next) out.push(n.value);
  return out;
}

// Reverse in place: turn each link around. No node is copied.
function reverse(head) {
  let prev = null;
  while (head !== null) {
    const next = head.next;
    head.next = prev;
    prev = head;
    head = next;
  }
  return prev;
}

const list = fromArray([1, 2, 3, 4]);
const last = list.next.next.next;
const rev = reverse(list);
console.log(toArray(rev), rev === last, list.next);

// Floyd's cycle test: a slow and a fast pointer meet only if the list has a loop.
function hasCycle(head) {
  let slow = head;
  let fast = head;
  while (fast !== null && fast.next !== null) {
    slow = slow.next;
    fast = fast.next.next;
    if (slow === fast) return true;
  }
  return false;
}

const loop = fromArray([1, 2, 3]);
console.log(hasCycle(loop));
loop.next.next.next = loop.next; // the last node now points back to the second node
console.log(hasCycle(loop));
```

```output
[ 4, 3, 2, 1 ] true null
false
true
```

- Verdict: idiom. JavaScript has no linked list type, so a list is a chain of plain objects. The reverse turns each link around, so the old last node becomes the head and the old head links to null. Floyd's test with a slow and a fast pointer finds the loop that the program makes.

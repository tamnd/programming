```javascript run id=pq
class PriorityQueue {
  #items = [];
  #before;

  constructor(before) {
    this.#before = before;
  }

  get size() {
    return this.#items.length;
  }

  peek() {
    return this.#items[0];
  }

  push(item) {
    const items = this.#items;
    items.push(item);
    let i = items.length - 1;
    while (i > 0) {
      const parent = (i - 1) >> 1;
      if (!this.#before(items[i], items[parent])) break;
      [items[i], items[parent]] = [items[parent], items[i]];
      i = parent;
    }
  }

  pop() {
    const items = this.#items;
    const top = items[0];
    const last = items.pop();
    if (items.length > 0) {
      items[0] = last;
      let i = 0;
      for (;;) {
        const left = 2 * i + 1;
        const right = left + 1;
        let best = i;
        if (left < items.length && this.#before(items[left], items[best])) best = left;
        if (right < items.length && this.#before(items[right], items[best])) best = right;
        if (best === i) break;
        [items[i], items[best]] = [items[best], items[i]];
        i = best;
      }
    }
    return top;
  }
}

const tasks = new PriorityQueue((a, b) => a.priority < b.priority);
const input = [[3, "update docs"], [1, "fix outage"], [2, "review patch"], [4, "plan sprint"]];
for (const [priority, name] of input) {
  tasks.push({ priority, name });
}

console.log("peek:", tasks.peek().name);
while (tasks.size > 0) {
  const { priority, name } = tasks.pop();
  console.log(priority, name);
}

const maxHeap = new PriorityQueue((a, b) => a > b);
for (const n of [3, 1, 2, 4]) maxHeap.push(n);
const order = [];
while (maxHeap.size > 0) order.push(maxHeap.pop());
console.log("largest first:", order.join(" "));
```

```output for=pq
peek: fix outage
1 fix outage
2 review patch
3 update docs
4 plan sprint
largest first: 4 3 2 1
```

- Verdict: idiom. JavaScript has no priority queue type. This class keeps a binary heap in an array, with a `before` function that decides the order.
- `push` moves the new item up, and `pop` moves the last item down from the root.
- The same class gives a max-heap when `before` uses `>`.

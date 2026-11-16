```javascript run id=queue
// An array with a head index: dequeue does not move the other items.
class Queue {
  #items = [];
  #head = 0;

  enqueue(item) {
    this.#items.push(item);
  }

  dequeue() {
    if (this.#head === this.#items.length) {
      return undefined;
    }
    const item = this.#items[this.#head];
    this.#items[this.#head] = undefined;
    this.#head++;
    if (this.#head === this.#items.length) {
      this.#items = [];
      this.#head = 0;
    }
    return item;
  }

  peek() {
    return this.#items[this.#head];
  }

  get size() {
    return this.#items.length - this.#head;
  }
}

const queue = new Queue();
for (const item of ["a", "b", "c"]) {
  queue.enqueue(item);
}
console.log("front", queue.peek(), "size", queue.size);
console.log(queue.dequeue(), "next", queue.peek(), "size", queue.size);
queue.dequeue();
queue.dequeue();
console.log("empty:", queue.size === 0 ? "yes" : "no");
console.log("dequeue on empty:", queue.dequeue());

// shift() also works, but it moves every other item.
const simple = ["a", "b", "c"];
console.log(simple.shift(), simple.join(","));
```

```output for=queue
front a size 3
a next b size 2
empty: yes
dequeue on empty: undefined
a b,c
```

- Verdict: idiom. JavaScript has no queue type. A class with an array and a head index dequeues in constant time.
- `shift()` is the short form, but it moves every remaining item, so it is slow for a large queue.

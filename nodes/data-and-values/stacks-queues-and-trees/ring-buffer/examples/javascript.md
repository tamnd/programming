```javascript run id=ring
class RingBuffer {
  #items;
  #start = 0;
  #count = 0;

  constructor(capacity) {
    this.#items = new Array(capacity);
  }

  push(item) {
    const capacity = this.#items.length;
    this.#items[(this.#start + this.#count) % capacity] = item;
    if (this.#count < capacity) {
      this.#count++;
    } else {
      this.#start = (this.#start + 1) % capacity;
    }
  }

  shift() {
    if (this.#count === 0) return undefined;
    const item = this.#items[this.#start];
    this.#start = (this.#start + 1) % this.#items.length;
    this.#count--;
    return item;
  }

  toArray() {
    const out = [];
    for (let i = 0; i < this.#count; i++) {
      out.push(this.#items[(this.#start + i) % this.#items.length]);
    }
    return out;
  }
}

const ring = new RingBuffer(3);
for (const reading of [10, 20, 30, 40, 50]) {
  ring.push(reading);
  console.log(`after ${reading}:`, ring.toArray().join(" "));
}

const oldest = ring.shift();
console.log(`read ${oldest}, left:`, ring.toArray().join(" "));
```

```output for=ring
after 10: 10
after 20: 10 20
after 30: 10 20 30
after 40: 20 30 40
after 50: 30 40 50
read 30, left: 40 50
```

- Verdict: idiom. JavaScript has no ring buffer type. This class keeps an array, a start index and a count.
- The write position is `(start + count) % capacity`. When the buffer is full, `push` overwrites the oldest item and moves the start.

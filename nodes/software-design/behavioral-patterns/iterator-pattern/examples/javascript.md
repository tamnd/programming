```javascript run id=tree
class Tree {
  #root = null;

  constructor(...keys) {
    for (const key of keys) this.insert(key);
  }

  insert(key) {
    const leaf = { key, left: null, right: null };
    if (this.#root === null) {
      this.#root = leaf;
      return;
    }
    let node = this.#root;
    for (;;) {
      const side = key < node.key ? "left" : "right";
      if (node[side] === null) {
        node[side] = leaf;
        return;
      }
      node = node[side];
    }
  }

  // A generator method: each call makes a fresh iterator.
  *[Symbol.iterator]() {
    const stack = [];
    let node = this.#root;
    while (stack.length > 0 || node !== null) {
      while (node !== null) {
        stack.push(node);
        node = node.left;
      }
      node = stack.pop();
      yield node.key;
      node = node.right;
    }
  }
}

// External iteration: the caller drives two iterators side by side.
function sameKeys(a, b) {
  const left = a[Symbol.iterator]();
  const right = b[Symbol.iterator]();
  for (;;) {
    const x = left.next();
    const y = right.next();
    if (x.done || y.done) return x.done && y.done;
    if (x.value !== y.value) return false;
  }
}

const a = new Tree(5, 3, 8, 1, 4, 9);
const b = new Tree(1, 3, 4, 5, 8, 9);
const c = new Tree(1, 3, 4, 5, 8);
console.log("in order:", ...a);
console.log("sum:", a[Symbol.iterator]().reduce((total, key) => total + key, 0));
console.log("first two:", ...a[Symbol.iterator]().take(2));
console.log("same keys as b:", sameKeys(a, b) ? "yes" : "no");
console.log("same keys as c:", sameKeys(a, c) ? "yes" : "no");
```

```output
in order: 1 3 4 5 8 9
sum: 30
first two: 1 3
same keys as b: yes
same keys as c: no
```

- Verdict: native. An object is iterable when it has a method under the key `Symbol.iterator` that returns an iterator, an object whose `next()` returns `{ value, done }`.
- A generator method (`*[Symbol.iterator]()`) gives the iterator with no hand-written `next`. The spread `...a` and `for...of` both call it.
- The generator object inherits from `Iterator.prototype`, so the iterator helpers `take` and `reduce` work on it. They are lazy: `take(2)` stops the traversal after two keys.
- `sameKeys` calls `next()` itself on two iterators. This is external iteration. A callback-style `forEach` could not compare two trees step by step.

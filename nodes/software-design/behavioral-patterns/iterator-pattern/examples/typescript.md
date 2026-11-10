```typescript run id=tree
type TreeNode = { key: number; left: TreeNode | null; right: TreeNode | null };

// The concrete iterator of Design Patterns, written by hand.
class InOrderIterator implements Iterator<number> {
  #stack: TreeNode[] = [];

  constructor(root: TreeNode | null) {
    this.#pushLeft(root);
  }

  #pushLeft(node: TreeNode | null): void {
    for (; node !== null; node = node.left) this.#stack.push(node);
  }

  next(): IteratorResult<number> {
    const node = this.#stack.pop();
    if (node === undefined) return { done: true, value: undefined };
    this.#pushLeft(node.right);
    return { done: false, value: node.key };
  }
}

// The aggregate. It makes a fresh iterator for each traversal.
class Tree implements Iterable<number> {
  #root: TreeNode | null = null;

  constructor(...keys: number[]) {
    for (const key of keys) this.insert(key);
  }

  insert(key: number): void {
    const leaf: TreeNode = { key, left: null, right: null };
    if (this.#root === null) {
      this.#root = leaf;
      return;
    }
    let node = this.#root;
    for (;;) {
      const next: TreeNode | null = key < node.key ? node.left : node.right;
      if (next === null) {
        if (key < node.key) node.left = leaf;
        else node.right = leaf;
        return;
      }
      node = next;
    }
  }

  [Symbol.iterator](): Iterator<number> {
    return new InOrderIterator(this.#root);
  }
}

function sameKeys(a: Iterable<number>, b: Iterable<number>): boolean {
  const left = a[Symbol.iterator]();
  const right = b[Symbol.iterator]();
  for (;;) {
    const x = left.next();
    const y = right.next();
    if (x.done || y.done) return Boolean(x.done && y.done);
    if (x.value !== y.value) return false;
  }
}

function* take<T>(source: Iterable<T>, count: number): Generator<T> {
  if (count <= 0) return;
  for (const item of source) {
    yield item;
    if (--count === 0) return;
  }
}

const a = new Tree(5, 3, 8, 1, 4, 9);
const b = new Tree(1, 3, 4, 5, 8, 9);
const c = new Tree(1, 3, 4, 5, 8);
let sum = 0;
for (const key of a) sum += key;
console.log("in order:", [...a].join(" "));
console.log("sum:", sum);
console.log("first two:", [...take(a, 2)].join(" "));
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

- Verdict: native, as in JavaScript. The types `Iterable<T>`, `Iterator<T>` and `IteratorResult<T>` describe the protocol, so the compiler checks the shape of `next()`.
- `InOrderIterator` is the hand-written form: the stack of nodes that are left is the state of one traversal. The generator method of the JavaScript example keeps the same state in a suspended call.
- `take` is a generator over any `Iterable<T>`. It returns before it asks for a third key, so the iterator of the tree does no more work.
- The `#stack` and `#root` fields are private at run time, so a client cannot reach the nodes through the tree or the iterator.

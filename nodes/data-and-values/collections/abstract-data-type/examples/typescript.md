```typescript run id=stack-adt
interface Stack<T> {
  push(v: T): void;
  pop(): T | undefined;
  isEmpty(): boolean;
}

class ArrayStack<T> implements Stack<T> {
  #items: T[] = [];
  push(v: T): void { this.#items.push(v); }
  pop(): T | undefined { return this.#items.pop(); }
  isEmpty(): boolean { return this.#items.length === 0; }
}

type Cell<T> = { v: T; rest: Cell<T> | null };

class LinkedStack<T> implements Stack<T> {
  #top: Cell<T> | null = null;
  push(v: T): void { this.#top = { v, rest: this.#top }; }
  pop(): T | undefined {
    if (this.#top === null) return undefined;
    const { v, rest } = this.#top;
    this.#top = rest;
    return v;
  }
  isEmpty(): boolean { return this.#top === null; }
}

function drain(s: Stack<number>): string {
  for (const v of [1, 2, 3]) s.push(v);
  const out: number[] = [];
  while (!s.isEmpty()) out.push(s.pop()!);
  return out.join(" ");
}

const stacks: Stack<number>[] = [new ArrayStack<number>(), new LinkedStack<number>()];
for (const s of stacks) {
  console.log(`${s.constructor.name}: ${drain(s)}`);
}
```

```output for=stack-adt
ArrayStack: 3 2 1
LinkedStack: 3 2 1
```

- Verdict: native. The interface `Stack<T>` names the operations, and each class `implements` it. The `#` fields keep the representation private when the program runs.
- `drain` takes a `Stack<number>`, so the checker allows only the three operations inside it.

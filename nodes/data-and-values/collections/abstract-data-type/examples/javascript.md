```javascript run id=stack-adt
class ArrayStack {
  #items = [];
  push(v) { this.#items.push(v); }
  pop() { return this.#items.pop(); }
  isEmpty() { return this.#items.length === 0; }
}

class LinkedStack {
  #top = null;
  push(v) { this.#top = { v, rest: this.#top }; }
  pop() { const { v, rest } = this.#top; this.#top = rest; return v; }
  isEmpty() { return this.#top === null; }
}

function drain(s) {
  for (const v of [1, 2, 3]) s.push(v);
  const out = [];
  while (!s.isEmpty()) out.push(s.pop());
  return out.join(" ");
}

for (const s of [new ArrayStack(), new LinkedStack()]) {
  console.log(`${s.constructor.name}: ${drain(s)}`);
}
console.log(Object.keys(new ArrayStack()).length);
```

```output for=stack-adt
ArrayStack: 3 2 1
LinkedStack: 3 2 1
0
```

- Verdict: native. A field whose name starts with `#` is private to the class. Code outside the class cannot read it, and `Object.keys` does not list it.
- JavaScript has no interface declaration. `drain` works with any object that has `push`, `pop` and `isEmpty`.

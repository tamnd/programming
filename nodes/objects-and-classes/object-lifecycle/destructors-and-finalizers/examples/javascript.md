```javascript run id=finalization-registry
"use strict";

// The callback may run some time after a Conn is collected, or never.
const registry = new FinalizationRegistry((name) => {
  console.log("safety net closes", name);
});

class Conn {
  #closed = false;

  constructor(name) {
    this.name = name;
    // The held value is the name, not the Conn: the callback must not keep it alive.
    registry.register(this, name, this);
  }

  close() {
    if (this.#closed) return;
    this.#closed = true;
    registry.unregister(this); // the program closed it: cancel the safety net
    console.log("closed", this.name);
  }

  [Symbol.dispose]() {
    this.close();
  }
}

const a = new Conn("a");
console.log("using", a.name);
a.close();
a.close();
console.log(registry.unregister(a)); // nothing left to cancel

{
  using b = new Conn("b"); // closed at the end of the block
  console.log("using", b.name);
}
console.log("end");
```

```output for=finalization-registry
using a
closed a
false
using b
closed b
end
```

- Verdict: stdlib. JavaScript has no destructors. `FinalizationRegistry` calls a callback some time after a registered object is collected, but an engine may call it late or never, so it can only be a safety net. The held value passed to the callback must not be the object itself, or the object stays reachable. Passing the object as the unregister token lets `close` cancel the callback. A `using` declaration calls `[Symbol.dispose]` at the end of its block, which gives deterministic cleanup.

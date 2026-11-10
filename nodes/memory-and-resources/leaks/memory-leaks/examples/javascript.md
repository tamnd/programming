```javascript run flags="--expose-gc" id=listeners
const page = new EventTarget(); // lives as long as the program
let resizeCalls = 0;

class Widget {
  constructor(name) {
    this.name = name;
    this.buffer = new Array(10_000).fill(0);
  }
  onResize() {
    resizeCalls += 1;
  }
}

// Counts the listeners by sending one event to them.
function countListeners() {
  resizeCalls = 0;
  page.dispatchEvent(new Event("resize"));
  return resizeCalls;
}

// Leaks: page keeps the listener, and the listener keeps the widget.
function mountLeaky(name) {
  const widget = new Widget(name);
  page.addEventListener("resize", () => widget.onResize());
  return new WeakRef(widget);
}

// Fixed: aborting the signal removes the listener.
function mountWithSignal(name, signal) {
  const widget = new Widget(name);
  page.addEventListener("resize", () => widget.onResize(), { signal });
  return new WeakRef(widget);
}

async function collect() {
  // A WeakRef target stays alive until the current job ends.
  await new Promise((resolve) => setTimeout(resolve, 0));
  globalThis.gc();
}

async function main() {
  const leaky = [];
  for (let i = 0; i < 5; i++) leaky.push(mountLeaky(`w${i}`));
  await collect();
  console.log("listeners on page:", countListeners());
  console.log("leaky widgets alive:", leaky.filter((r) => r.deref()).length);

  const controller = new AbortController();
  const fixed = [];
  for (let i = 0; i < 5; i++) fixed.push(mountWithSignal(`f${i}`, controller.signal));
  console.log("listeners on page:", countListeners());
  controller.abort();
  await collect();
  console.log("listeners on page after abort:", countListeners());
  console.log("fixed widgets alive:", fixed.filter((r) => r.deref()).length);

  // WeakMap: data for each object that does not keep the object alive.
  const sizes = new WeakMap();
  let widget = new Widget("w-map");
  const probe = new WeakRef(widget);
  sizes.set(widget, { width: 640 });
  widget = null;
  await collect();
  console.log("WeakMap key collected:", probe.deref() === undefined);
}

main();
```

```output
listeners on page: 5
leaky widgets alive: 5
listeners on page: 10
listeners on page after abort: 5
fixed widgets alive: 0
WeakMap key collected: true
```

- Verdict: native. The engine collects unreachable objects. A listener on a long-lived target is reachable, and so is each object that its closure captures.
- `countListeners` sends one event and counts the calls, so the output does not depend on an engine API.
- The `signal` option removes all the listeners of a component with one `abort()` call. `removeEventListener` works too, but it needs the same function object that `addEventListener` got.
- In a heap snapshot, the retainers of a leaked widget show the path from `page` through the listener.

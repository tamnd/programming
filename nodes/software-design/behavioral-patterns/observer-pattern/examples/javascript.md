```javascript run id=sensor
class Sensor extends EventTarget {
  set(value) {
    this.dispatchEvent(new CustomEvent("reading", { detail: value }));
  }
}

const sensor = new Sensor();
const displayScope = new AbortController();

sensor.addEventListener(
  "reading",
  (event) => console.log(`display: ${event.detail.toFixed(1)}`),
  { signal: displayScope.signal },
);
sensor.addEventListener("reading", (event) => {
  if (event.detail > 30) {
    console.log(`alarm: ${event.detail.toFixed(1)} is above 30`);
  }
});

sensor.set(21.5);
sensor.set(30.5);
displayScope.abort();
sensor.set(31.0);
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: stdlib. `EventTarget` is part of the DOM Standard, and Node.js has the same class as a global. A class that extends it gets `addEventListener`, `removeEventListener` and `dispatchEvent`.
- `dispatchEvent` is synchronous. It calls the listeners in the order of registration and returns after the last one.
- The `signal` option removes the listener when the `AbortController` aborts. One controller can remove many listeners at once, and the code does not need to keep a reference to each listener function.
- Node.js also has `EventEmitter` in `node:events`, with `on`, `off` and `emit`. Its listeners get the arguments of `emit` directly, with no event object.

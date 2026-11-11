```javascript run id=emitter
import { EventEmitter } from "node:events";

const door = new EventEmitter();
door.on("open", (who) => console.log(`handler 1: ${who} opened the door`));
door.once("open", () => console.log("handler 2: runs once"));

setTimeout(() => console.log("timer"), 0);
Promise.resolve().then(() => console.log("microtask"));
door.emit("open", "ana");
door.emit("open", "bo");
console.log("end of script, listeners:", door.listenerCount("open"));
```

```output for=emitter
handler 1: ana opened the door
handler 2: runs once
handler 1: bo opened the door
end of script, listeners: 1
microtask
timer
```

```javascript wrong id=busy-loop
const start = Date.now();
setTimeout(() => console.log("timer late by at least 200 ms:", Date.now() - start >= 250), 50);
while (Date.now() - start < 300) {
  // a long computation in one job
}
```

```output for=busy-loop
timer late by at least 200 ms: true
```

- Verdict: native. JavaScript runs in an event loop in every host. Browsers dispatch DOM events to listeners from `addEventListener`, and Node.js has `EventEmitter` for events of its own objects.
- `emit` calls the listeners at once, in the order they were added. It does not go through the event loop. The promise callback runs as a microtask when the script ends, and the timer runs in a later turn of the loop.
- In the wrong block, the timer is due after 50 ms, but the loop cannot run it until the current job ends. Split long work into pieces, or move it to a worker.

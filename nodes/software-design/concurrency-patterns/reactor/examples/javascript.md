```javascript run id=reactor
class Reactor {
  constructor() {
    this.handlers = new Map();
  }
  register(handle, handler) {
    this.handlers.set(handle, handler);
  }
  remove(handle) {
    this.handlers.delete(handle);
  }
  run(ready) {
    // ready stands in for select: [handle, data] pairs in arrival order
    for (const [handle, data] of ready) {
      this.handlers.get(handle)(handle, data);
    }
  }
}

const reactor = new Reactor();

function echo(handle, data) {
  if (data) {
    console.log(`${handle}: ${data}`);
  } else {
    console.log("close", handle);
    reactor.remove(handle);
  }
}

reactor.register("listener", (handle, newHandle) => {
  console.log("accept", newHandle);
  reactor.register(newHandle, echo);
});

reactor.run([
  ["listener", "conn-1"], ["listener", "conn-2"],
  ["conn-1", "hello"], ["conn-2", "hi"],
  ["conn-1", ""], ["conn-2", ""],
]);
```

```output
accept conn-1
accept conn-2
conn-1: hello
conn-2: hi
close conn-1
close conn-2
```

- Verdict: native. A handler is a function value in a `Map`.

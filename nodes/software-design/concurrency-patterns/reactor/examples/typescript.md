```typescript run id=reactor
type Handler = (handle: string, data: string) => void;

class Reactor {
  private handlers = new Map<string, Handler>();
  register(handle: string, handler: Handler): void {
    this.handlers.set(handle, handler);
  }
  remove(handle: string): void {
    this.handlers.delete(handle);
  }
  run(ready: Array<[string, string]>): void {
    // ready stands in for select: [handle, data] pairs in arrival order
    for (const [handle, data] of ready) {
      const handler = this.handlers.get(handle);
      if (handler) handler(handle, data);
    }
  }
}

const reactor = new Reactor();

const echo: Handler = (handle, data) => {
  if (data) {
    console.log(`${handle}: ${data}`);
  } else {
    console.log("close", handle);
    reactor.remove(handle);
  }
};

reactor.register("listener", (_handle, newHandle) => {
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

- Verdict: native. The type `Handler` is the hook method as a function type. `Map.get` can return `undefined`, so the dispatcher checks the handler before it calls it.

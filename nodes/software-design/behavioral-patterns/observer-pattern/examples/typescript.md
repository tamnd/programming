```typescript run id=sensor
type Observer<T> = (value: T) => void;

class Subject<T> {
  #observers = new Set<Observer<T>>();

  subscribe(observer: Observer<T>): Disposable {
    this.#observers.add(observer);
    return { [Symbol.dispose]: () => this.#observers.delete(observer) };
  }

  notify(value: T): void {
    for (const observer of [...this.#observers]) {
      observer(value);
    }
  }
}

const sensor = new Subject<number>();
sensor.subscribe((value) => {
  if (value > 30) {
    console.log(`alarm: ${value.toFixed(1)} is above 30`);
  }
});

{
  using display = sensor.subscribe((value) => {
    console.log(`display: ${value.toFixed(1)}`);
  });
  sensor.notify(21.5);
  sensor.notify(30.5);
}
sensor.notify(31.0);
```

```output for=sensor
display: 21.5
alarm: 30.5 is above 30
display: 30.5
alarm: 31.0 is above 30
```

- Verdict: stdlib. The type checker adds generic observer types to the JavaScript forms: `EventTarget`, `EventEmitter` or a hand-written subject such as this one.
- `subscribe` returns a `Disposable`. The `using` declaration calls its `[Symbol.dispose]` method when the block ends, so the display is removed at the closing brace, even when the block throws.
- The observers live in a `Set`, so the same function cannot subscribe twice. A `Set` keeps the order of insertion, so the order of notification is the order of subscription.
- The loop runs over a copy of the set, so an observer can unsubscribe during a notification.

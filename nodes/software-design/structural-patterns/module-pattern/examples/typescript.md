```typescript run id=counter-module
interface Counter {
  increment(): void;
  decrement(): void;
  value(): number;
}

const counter: Counter = (function (): Counter {
  let privateCounter = 0;
  function changeBy(val: number): void {
    privateCounter += val;
  }
  return {
    increment() {
      changeBy(1);
    },
    decrement() {
      changeBy(-1);
    },
    value() {
      return privateCounter;
    },
  };
})();

console.log("value:", counter.value());
counter.increment();
counter.increment();
console.log("value:", counter.value());
counter.decrement();
console.log("value:", counter.value());
```

```output
value: 0
value: 2
value: 1
```

- Verdict: idiom. The `Counter` interface is the public API, and the compiler rejects `counter.privateCounter`. ES modules are the modern form.

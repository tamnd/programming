```javascript run id=counter-module
const counter = (function () {
  let privateCounter = 0;
  function changeBy(val) {
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

- Verdict: idiom. The IIFE is the classic form. In code that uses ES modules, a file with `let privateCounter` and `export function increment()` gives the same privacy with no wrapper.

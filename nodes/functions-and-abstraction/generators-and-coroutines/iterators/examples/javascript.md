```javascript run id=countdown
class Countdown {
  constructor(start) {
    this.start = start;
  }

  [Symbol.iterator]() {
    let current = this.start;
    return {
      next() {
        if (current <= 0) {
          return { done: true, value: undefined };
        }
        return { done: false, value: current-- };
      },
      return() {
        console.log("closed");
        return { done: true, value: undefined };
      },
    };
  }
}

for (const n of new Countdown(3)) {
  console.log(n);
}

for (const n of new Countdown(3)) {
  if (n === 2) {
    break;
  }
  console.log(n);
}

console.log([...new Countdown(2)]);
```

```output for=countdown
3
2
1
3
closed
[ 2, 1 ]
```

- Verdict: native. An iterable has a method under the key `Symbol.iterator`. The method returns an iterator, an object whose `next()` returns `{ done, value }`.
- `for...of`, spread syntax, destructuring and `Array.from` all use the same protocol.
- The optional `return()` method runs when the loop stops early. Here `break` calls it and prints `closed`. A loop that runs to the end does not call it.
- A generator function (`function*`) makes an iterator with less code.

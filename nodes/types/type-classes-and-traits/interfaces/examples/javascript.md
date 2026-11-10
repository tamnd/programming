```javascript run id=iterable
class Countdown {
  constructor(start) {
    this.start = start;
  }

  [Symbol.iterator]() {
    let n = this.start;
    return {
      next: () => (n > 0 ? { value: n--, done: false } : { value: undefined, done: true }),
    };
  }
}

console.log([...new Countdown(3)]);
for (const n of new Countdown(2)) {
  console.log(n);
}

function isIterable(value) {
  return value != null && typeof value[Symbol.iterator] === "function";
}

console.log(isIterable(new Countdown(1)), isIterable([1]), isIterable({}));

const plain = { start: 3 };
try {
  console.log([...plain]);
} catch (error) {
  console.log(`${error.name}: ${error.message}`);
}
```

```output
[ 3, 2, 1 ]
2
1
true true false
TypeError: plain is not iterable
```

- Verdict: idiom. JavaScript has no `interface` declaration. An object conforms to a protocol when it has the right properties, and code checks them at run time if it checks at all.
- The language specification itself uses this idea. The iterable interface is the method at the key `Symbol.iterator`. `Countdown` names no interface, but spread syntax and `for...of` accept it.
- An object without the method fails only when it is used, with a `TypeError`.

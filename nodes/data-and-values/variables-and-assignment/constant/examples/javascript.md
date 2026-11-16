```javascript run id=const
const RATE = 0.2;
const config = { retries: 3 };
config.retries = 5;
console.log(RATE, config.retries);

const frozen = Object.freeze({ retries: 3 });
console.log(Reflect.set(frozen, "retries", 5), frozen.retries, Object.isFrozen(frozen));
```

```output for=const
0.2 5
false 3 true
```

- Verdict: native. `const` stops a new assignment to the name, but not a change to the object. `Object.freeze` stops changes to the properties of the object. `Reflect.set` returns `false` for the failed write. A plain assignment to a frozen property does nothing outside strict mode, and throws a `TypeError` in strict mode.

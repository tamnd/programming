```javascript run id=either
function describe(value) {
  if (typeof value === "number") {
    return `number ${value}, doubled ${value * 2}`;
  }
  if (typeof value === "string") {
    return `text ${value}, upper ${value.toUpperCase()}`;
  }
  throw new TypeError(`not a number or a string: ${typeof value}`);
}

for (const v of [21, "ok"]) {
  console.log(describe(v));
}
try {
  describe(true);
} catch (e) {
  console.log("error:", e.message);
}
```

```output for=either
number 21, doubled 42
text ok, upper OK
error: not a number or a string: boolean
```

- Verdict: idiom. JavaScript has no static types, so any variable can hold a value of any type. `typeof` tells them apart.
- Nothing stops a caller from passing a third type, so the function checks for it and throws.

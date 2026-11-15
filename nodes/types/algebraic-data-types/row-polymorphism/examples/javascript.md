```javascript run id=keep
const shift = (p) => ({ ...p, x: p.x + 1 });

const a = shift({ x: 1, y: 2 });
const b = shift({ x: 1, y: 2, label: "home" });
console.log(`point: x=${a.x} y=${a.y}`);
console.log(`labelled: x=${b.x} y=${b.y} label=${b.label}`);
console.log("keys:", Object.keys(b).join(" "));
```

```output for=keep
point: x=2 y=2
labelled: x=2 y=2 label=home
keys: x y label
```

- Verdict: idiom. JavaScript has no static types, so any object with an `x` works. The spread `...p` copies all the other properties into the new object.
- Nothing checks that the argument has `x` and `y` before the program runs. A missing `x` gives `NaN`, not an error.

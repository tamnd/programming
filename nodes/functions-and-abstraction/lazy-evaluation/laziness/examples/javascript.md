```javascript run id=generators
function* naturals() {
  for (let n = 1; ; n++) {
    console.log("computing", n);
    yield n;
  }
}

function* take(items, count) {
  if (count <= 0) return;
  for (const item of items) {
    yield item;
    if (--count === 0) return;
  }
}

const firstTwo = take(naturals(), 2);
console.log("nothing computed yet");
console.log([...firstTwo]);

const settings = {
  get theme() {
    console.log("loading theme");
    const value = "dark";
    Object.defineProperty(this, "theme", { value });
    return value;
  },
};
console.log(settings.theme, settings.theme);
```

```output for=generators
nothing computed yet
computing 1
computing 2
[ 1, 2 ]
loading theme
dark dark
```

- Verdict: native. Generator functions produce values on demand, and the iterator helpers of newer engines, such as `Iterator.prototype.take`, chain lazy steps.
- A getter that replaces itself with a plain property is the usual lazy and cached property. `&&`, `||` and `??` evaluate the right operand only when they need it.

```javascript run id=light
const Light = Object.freeze({ RED: 0, AMBER: 1, GREEN: 2 });

const nameOf = (value) => Object.keys(Light).find((k) => Light[k] === value).toLowerCase();

function after(light) {
  switch (light) {
    case Light.RED:
      return Light.GREEN;
    case Light.GREEN:
      return Light.AMBER;
    case Light.AMBER:
      return Light.RED;
    default:
      throw new RangeError(`no light ${light}`);
  }
}

for (const [name, value] of Object.entries(Light)) {
  console.log(name.toLowerCase(), value, "then", nameOf(after(value)));
}
console.log("frozen:", Object.isFrozen(Light) ? "yes" : "no");
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
frozen: yes
```

- Verdict: idiom. JavaScript has no enum. A frozen object maps names to values, and `Object.entries` lists them in insertion order.
- Nothing stops a function from receiving a number that is not in the object, so `after` throws for an unknown value.
- String values, such as `"red"`, are also common, because they print well.

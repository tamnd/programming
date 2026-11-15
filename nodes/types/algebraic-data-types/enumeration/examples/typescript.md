```typescript run id=light
const Light = { Red: 0, Amber: 1, Green: 2 } as const;
type Light = (typeof Light)[keyof typeof Light]; // 0 | 1 | 2

const names = ["red", "amber", "green"] as const;

function after(light: Light): Light {
  switch (light) {
    case Light.Red:
      return Light.Green;
    case Light.Green:
      return Light.Amber;
    case Light.Amber:
      return Light.Red;
  }
}

for (const light of Object.values(Light)) {
  console.log(`${names[light]} ${light} then ${names[after(light)]}`);
}
console.log("keys: " + Object.keys(Light).join(" "));
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
keys: Red Amber Green
```

- Verdict: native. TypeScript has `enum`, but an object with `as const` is often enough, and this example uses that form.
- `as const` keeps the literal values, and the type `Light` is the union `0 | 1 | 2`. A call such as `after(7)` does not compile.
- The `switch` covers every value, so the type checker accepts `after` with no final `return`.

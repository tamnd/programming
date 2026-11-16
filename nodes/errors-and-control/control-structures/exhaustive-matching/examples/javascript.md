```javascript run id=exhaustive-matching-javascript
const Light = Object.freeze({ RED: "red", YELLOW: "yellow", GREEN: "green" });

function action(light) {
  switch (light) {
    case Light.RED:
      return "stop";
    case Light.YELLOW:
      return "slow down";
    case Light.GREEN:
      return "go";
    default:
      throw new Error(`unknown light: ${light}`);
  }
}

for (const light of Object.values(Light)) {
  console.log(`${light}: ${action(light)}`);
}
```

```output for=exhaustive-matching-javascript
red: stop
yellow: slow down
green: go
```

- Verdict: absent. JavaScript has no static types, so nothing checks that a switch covers each value. A `default` clause that throws finds a missing case at run time, and only when that value comes.

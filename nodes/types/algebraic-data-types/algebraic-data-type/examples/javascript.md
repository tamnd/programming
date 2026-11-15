```javascript run id=count
const lights = ["red", "amber", "green"];
const flags = [false, true];
const yn = (b) => (b ? "yes" : "no");

// Product: an object with a flag AND a light.
const pairs = flags.flatMap((flag) => lights.map((light) => ({ flag, light })));

// Sum: an object with a kind field that says which case it is.
const choices = [
  ...flags.map((flag) => ({ kind: "flag", flag })),
  ...lights.map((light) => ({ kind: "light", light })),
];

function show(c) {
  switch (c.kind) {
    case "flag":
      return "flag:" + yn(c.flag);
    case "light":
      return "light:" + c.light;
    default:
      throw new Error("unknown kind " + c.kind);
  }
}

console.log("pairs: " + pairs.length);
console.log(pairs.map((p) => yn(p.flag) + "/" + p.light).join(" "));
console.log("choices: " + choices.length);
console.log(choices.map(show).join(" "));
```

```output for=count
pairs: 6
no/red no/amber no/green yes/red yes/amber yes/green
choices: 5
flag:no flag:yes light:red light:amber light:green
```

- Verdict: idiom. An object is a product. An object with a `kind` field is a sum by convention only.
- Nothing checks the cases. The `default` branch that throws makes a wrong `kind` fail at once.

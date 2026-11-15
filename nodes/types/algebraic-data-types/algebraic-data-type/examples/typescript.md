```typescript run id=count
type Light = "red" | "amber" | "green";
const lights: Light[] = ["red", "amber", "green"];
const flags = [false, true];

// Product: a flag AND a light.
type Pair = { flag: boolean; light: Light };

// Sum: a flag OR a light. The kind field is the discriminant.
type Choice = { kind: "flag"; flag: boolean } | { kind: "light"; light: Light };

const yn = (b: boolean): string => (b ? "yes" : "no");

function show(c: Choice): string {
  switch (c.kind) {
    case "flag":
      return "flag:" + yn(c.flag);
    case "light":
      return "light:" + c.light;
  }
}

const pairs: Pair[] = flags.flatMap((flag) => lights.map((light) => ({ flag, light })));
const choices: Choice[] = [
  ...flags.map((flag): Choice => ({ kind: "flag", flag })),
  ...lights.map((light): Choice => ({ kind: "light", light })),
];

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

- Verdict: native. An object type is a product, and a union of object types with a common literal `kind` field is a discriminated union, a sum.
- Inside `case "flag"`, the type checker narrows `c` to the flag case, so `c.light` there is a type error.
- `show` needs no final `return`, because the two cases cover the union.

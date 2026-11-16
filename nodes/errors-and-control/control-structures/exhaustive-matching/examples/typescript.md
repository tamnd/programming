```typescript run id=exhaustive-matching-typescript
type Light = "red" | "yellow" | "green";

function action(light: Light): string {
  switch (light) {
    case "red":
      return "stop";
    case "yellow":
      return "slow down";
    case "green":
      return "go";
    default: {
      // If a case is missing, light is not never here, and tsc reports an error.
      const unreachable: never = light;
      throw new Error(`unknown light: ${unreachable}`);
    }
  }
}

for (const light of ["red", "yellow", "green"] as const) {
  console.log(`${light}: ${action(light)}`);
}
```

```output for=exhaustive-matching-typescript
red: stop
yellow: slow down
green: go
```

- Verdict: idiom. TypeScript does not require a switch to be exhaustive. Narrowing makes the type of `light` in the `default` clause `never` when each member of the union has a case. The assignment to a `never` variable fails to compile when a member is missing, so it works as an exhaustiveness check.

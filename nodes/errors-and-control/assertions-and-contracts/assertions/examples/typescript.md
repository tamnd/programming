```typescript run id=narrow
function assert(condition: unknown, message: string): asserts condition {
  if (!condition) {
    throw new Error("assertion failed: " + message);
  }
}

function assertIsString(value: unknown): asserts value is string {
  assert(typeof value === "string", `expected a string, got ${typeof value}`);
}

function shout(value: unknown): string {
  assertIsString(value);
  return value.toUpperCase();
}

console.log(shout("ok"));
try {
  console.log(shout(42));
} catch (err) {
  console.log((err as Error).message);
}
```

```output
OK
assertion failed: expected a string, got number
```

- Verdict: idiom. TypeScript adds no run-time check. The program uses an ordinary function that throws, or the module `node:assert`.
- The return type `asserts condition` tells the checker that the condition is true after the call returns. `asserts value is string` narrows the type of `value`, so `value.toUpperCase()` type checks.
- The checker trusts the signature and does not check the body. The block below shows the result of a body that does not throw.

This assertion function only logs. The checker still narrows `value` to `string` after the call, and the program fails when it calls a string method on a number.

```typescript fails expect=exception:TypeError id=lying-assert
function assertIsString(value: unknown): asserts value is string {
  if (typeof value !== "string") {
    console.log("not a string:", typeof value);
  }
}

function shout(value: unknown): string {
  assertIsString(value);
  return value.toUpperCase();
}

console.log(shout(42));
```

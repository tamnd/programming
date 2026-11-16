```typescript run id=strict
function trace<T>(label: string, value: T): T {
  console.log("evaluating", label);
  return value;
}

function pick(flag: boolean, left: () => number, right: () => number): number {
  console.log("inside pick");
  return flag ? left() : right();
}

console.log(pick(trace("flag", true), () => trace("left", 1), () => trace("right", 2)));
```

```output for=strict
evaluating flag
inside pick
evaluating left
1
```

- Verdict: native. TypeScript has the evaluation rules of JavaScript: arguments are evaluated from left to right before the call.
- The type `() => number` marks a parameter as a thunk. The caller passes a function, and the body decides if it calls it. Here only `left` runs, after `inside pick`.

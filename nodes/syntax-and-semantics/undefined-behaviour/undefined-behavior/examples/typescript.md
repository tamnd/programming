```typescript run id=defined
const scores: number[] = [3, 1, 2];
const sorted = [...scores].sort((a, b) => a - b);
console.log(sorted);

const fourth: number | undefined = scores[3];
console.log(fourth ?? "no fourth score");
```

```output for=defined
[ 1, 2, 3 ]
no fourth score
```

- Verdict: absent. TypeScript compiles to JavaScript and has the same run-time rules.
- The type checker rejects the boolean comparator of the JavaScript example with error TS2345, because the comparator must return a number. The runner of this page strips the types without a check, so the page cannot show the error as a `fails` block.
- By default, the type of `scores[3]` is `number`, but the value is `undefined`. The compiler option `noUncheckedIndexedAccess` adds `undefined` to the type of an index access.

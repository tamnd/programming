```typescript run id=fall-through-typescript
function rights(level: number): string[] {
  const out: string[] = [];
  switch (level) {
    case 3:
      out.push("delete");
    // falls through
    case 2:
      out.push("write");
    // falls through
    case 1:
      out.push("read");
      break;
    default:
      out.push("none");
  }
  return out;
}

console.log(rights(3).join(" "));
console.log(rights(1).join(" "));
```

```output for=fall-through-typescript
delete write read
read
```

- Verdict: native. TypeScript has the switch of JavaScript, with the same fall-through. The compiler option `noFallthroughCasesInSwitch` reports an error for each non-empty case that does not end with `break`, `return` or `throw`.

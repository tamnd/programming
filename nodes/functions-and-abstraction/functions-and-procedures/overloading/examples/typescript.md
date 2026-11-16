```typescript run id=overloading-typescript
function show(x: number): string;
function show(x: string): string;
function show(x: number, y: number): string;
function show(x: number | string, y?: number): string {
  if (typeof x === "string") {
    return `string ${x}`;
  }
  if (y === undefined) {
    return `int ${x}`;
  }
  return `two ints ${x} ${y}`;
}

console.log(show(42));
console.log(show("hi"));
console.log(show(3, 4));
```

```output for=overloading-typescript
int 42
string hi
two ints 3 4
```

- Verdict: native. The overload signatures give the type checker three ways to call `show`. One implementation handles all of them, and callers cannot call the implementation signature.

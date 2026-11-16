```typescript run id=function-typescript
function rectangleArea(width: number, height: number): number {
  return width * height;
}

function greeting(name: string): string {
  return "Hello, " + name + "!";
}

console.log(rectangleArea(3, 4));
console.log(greeting("Ada"));
```

```output for=function-typescript
12
Hello, Ada!
```

- Verdict: native. The same functions as in JavaScript, with a type on each parameter and on the result.

```typescript run id=function-call-typescript
function rectangleArea(width: number, height: number): number {
  console.log("computing", width, "by", height);
  return width * height;
}

const area: number = rectangleArea(3, 4);
console.log("area", area);
```

```output for=function-call-typescript
computing 3 by 4
area 12
```

- Verdict: native. The call syntax is the same as in JavaScript. The compiler checks the number and the types of the arguments.

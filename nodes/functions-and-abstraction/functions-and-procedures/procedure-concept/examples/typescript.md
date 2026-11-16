```typescript run id=procedure-typescript
function greet(name: string): void {
  console.log("Hello, " + name + "!");
}

greet("Ada");
greet("Alan");
```

```output for=procedure-typescript
Hello, Ada!
Hello, Alan!
```

- Verdict: native. The result type `void` marks a function that gives back no value.

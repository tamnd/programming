```javascript run id=procedure-javascript
function greet(name) {
  console.log("Hello, " + name + "!");
}

greet("Ada");
const result = greet("Alan");
console.log(result);
```

```output for=procedure-javascript
Hello, Ada!
Hello, Alan!
undefined
```

- Verdict: native. A function with no `return` statement is a procedure, and its call gives back `undefined`.

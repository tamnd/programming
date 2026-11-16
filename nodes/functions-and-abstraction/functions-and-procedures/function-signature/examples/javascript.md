```javascript run id=function-signature-javascript
function average(a, b) {
  return (a + b) / 2;
}

console.log(average(3, 4));
```

```output for=function-signature-javascript
3.5
```

- Verdict: native. The signature has only the name and the parameter names. JavaScript does not check the number or the types of the arguments.

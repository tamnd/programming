```javascript run id=overloading-javascript
function show(x, y) {
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

```output for=overloading-javascript
int 42
string hi
two ints 3 4
```

- Verdict: absent. JavaScript has no overloading. One function checks the number of arguments and the type of the first one.

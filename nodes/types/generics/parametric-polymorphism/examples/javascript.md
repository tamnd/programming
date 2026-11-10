```javascript run id=first
function first(items) {
  return items[0];
}

console.log(first([3, 1, 2]));
console.log(first("abc"));
console.log(first([true, false]));

const numbers = [1, 2];
numbers.push("three");
console.log(numbers.map((x) => typeof x).join(", "));
```

```output for=first
3
a
true
number, number, string
```

- Verdict: n/a. JavaScript has no static types, so it has no type parameters. Every function already accepts values of any type.
- The language checks types only when an operation runs. An array accepts elements of any type, so nothing stops a string in an array of numbers.
- The effect of generics in JavaScript is the duck typing of [[static-and-dynamic-typing]]: `first` works on any value that has an element at index 0.
- TypeScript adds type parameters on top of JavaScript and removes them before the code runs (see the TypeScript example).

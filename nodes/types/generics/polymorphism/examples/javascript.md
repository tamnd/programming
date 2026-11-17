```javascript run id=duck-coercion
// The objects share no class; each has an area method.
const shapes = [
  { side: 2, area() { return this.side * this.side; } },
  { r: 1, area() { return Math.round(Math.PI * this.r * this.r * 100) / 100; } },
];
console.log(shapes.map((s) => s.area()));

// One operator, several types: + adds numbers and joins strings.
console.log(2 + 3, "a" + "b");

// Coercion: the number is converted to a string, or the string to a number.
console.log("5" + 9, "5" * 9, Number("5") + 9);
```

```output for=duck-coercion
[ 4, 3.14 ]
5 ab
59 45 14
```

- Verdict: native. JavaScript finds the method on the object when the program runs, so any object with `area` works. `+` adds two numbers and joins two strings. With a string and a number, `+` converts the number to a string, but `*` converts the string to a number. `Number("5")` makes the conversion explicit.

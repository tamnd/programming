```javascript run id=function-javascript
function rectangleArea(width, height) {
  return width * height;
}

const greeting = (name) => "Hello, " + name + "!";

console.log(rectangleArea(3, 4));
console.log(greeting("Ada"));
```

```output for=function-javascript
12
Hello, Ada!
```

- Verdict: native. A function declaration starts with `function`. An arrow function such as `greeting` is a function value that the program stores in a constant.

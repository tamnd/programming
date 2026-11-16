```javascript run id=function-call-javascript
function rectangleArea(width, height) {
  console.log("computing", width, "by", height);
  return width * height;
}

const area = rectangleArea(3, 4);
console.log("area", area);
```

```output for=function-call-javascript
computing 3 by 4
area 12
```

- Verdict: native. A call writes the arguments in parentheses after the name. JavaScript does not check the number of arguments.

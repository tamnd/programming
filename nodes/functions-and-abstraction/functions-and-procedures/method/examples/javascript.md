```javascript run id=method-javascript
class Rectangle {
  constructor(width, height) {
    this.width = width;
    this.height = height;
  }

  area() {
    return this.width * this.height;
  }

  static square(side) {
    return new Rectangle(side, side);
  }
}

const r = new Rectangle(3, 4);
console.log(r.area());
console.log(Rectangle.square(5).area());
```

```output for=method-javascript
12
25
```

- Verdict: native. `area` is a method on the class, and its receiver is `this`. `square` is a static method, which the program calls on the class.

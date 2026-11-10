```javascript run id=hierarchy
class Shape {
  area() {
    return 0;
  }
  describe() {
    return `${this.constructor.name} ${this.area()}`;
  }
}

class Square extends Shape {
  constructor(side) {
    super();
    this.side = side;
  }
  area() {
    return this.side * this.side;
  }
}

const square = new Square(3);
console.log(square instanceof Square, square instanceof Shape);
console.log(square.describe());
console.log(Object.getPrototypeOf(Square.prototype) === Shape.prototype);

const plain = { area: () => 7 };
console.log(plain instanceof Shape);
Object.setPrototypeOf(plain, Shape.prototype);
console.log(plain instanceof Shape, plain.describe());
```

```output for=hierarchy
true true
Square 9
true
false
true Shape 7
```

- Verdict: native. `class Square extends Shape` sets the prototype of `Square.prototype` to `Shape.prototype`, so a `Square` inherits the methods of `Shape`.
- `instanceof` walks the prototype chain of the object. It says nothing about the members of the object.
- `Object.setPrototypeOf` changes the chain at run time, so a plain object becomes an instance of `Shape`.
- No static check exists. Any object with the right methods works in a call, which is duck typing.

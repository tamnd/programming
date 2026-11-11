```javascript run id=shapes
class Shape {
  describe() {
    return `${this.constructor.name} with area ${this.area().toFixed(2)}`;
  }
}

class Circle extends Shape {
  constructor(r) { super(); this.r = r; }
  area() { return Math.PI * this.r ** 2; }
}

class Square extends Shape {
  constructor(side) { super(); this.side = side; }
  area() { return this.side ** 2; }
}

for (const s of [new Circle(1), new Square(2)]) console.log(s.describe());

// The lookup walks the prototype chain at the time of the call.
const sq = new Square(3);
console.log(Object.hasOwn(sq, "area"), Object.hasOwn(Square.prototype, "area"),
  Object.hasOwn(Shape.prototype, "describe"));
Square.prototype.area = function () { return -1; }; // change the method for every square
console.log(sq.area());
```

```output for=shapes
Circle with area 3.14
Square with area 4.00
false true true
-1
```

```javascript wrong id=constructor-call
class Shape {
  constructor() {
    this.label = this.describe(); // dispatches to the subclass too early
  }
  describe() { return "shape"; }
}

class Circle extends Shape {
  r = 2; // a field of Circle: set only after the Shape constructor returns
  describe() { return `circle with radius ${this.r}`; }
}

console.log(new Circle().label);
```

```output for=constructor-call
circle with radius undefined
```

- Verdict: native. A method call `s.area()` looks up the property `area` on the object and then along its prototype chain, at the time of the call. The methods of a class live on its prototype, which is shared by all instances, so changing `Square.prototype.area` changes the method for every square, even one made before the change.
- The wrong block calls an overridable method from a constructor. The call dispatches to `Circle.describe`, but the fields of `Circle` are set only after the constructor of `Shape` returns, so `this.r` is still `undefined`. Do not call methods that a subclass can override from a constructor.

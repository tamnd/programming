```javascript run id=window-shape
class Rectangle {
  constructor(width, height) {
    this.width = width;
    this.height = height;
  }
  area() {
    return this.width * this.height;
  }
  describe(owner) {
    return `${owner.title}: a rectangle of ${this.width} by ${this.height}`;
  }
}

class Triangle {
  constructor(base, height) {
    this.base = base;
    this.height = height;
  }
  area() {
    return Math.floor((this.base * this.height) / 2);
  }
  describe(owner) {
    return `${owner.title}: a triangle of ${this.base} by ${this.height}`;
  }
}

class AppWindow {
  constructor(title, shape) {
    this.title = title;
    this.shape = shape; // the delegate
  }
  area() {
    return this.shape.area(); // forward
  }
  describe() {
    return this.shape.describe(this); // delegate, and pass the receiver
  }
}

const w = new AppWindow("main", new Rectangle(3, 4));
console.log("area:", w.area());
console.log(w.describe());
w.shape = new Triangle(3, 4); // change the behaviour at run time
console.log("area:", w.area());
console.log(w.describe());
```

```output
area: 12
main: a rectangle of 3 by 4
area: 6
main: a triangle of 3 by 4
```

- Verdict: native for prototypes, idiom for classes. A method found on the prototype runs with `this` set to the object that received the call, which is delegation in the strict sense. Between two class instances, the window forwards by hand, as here.

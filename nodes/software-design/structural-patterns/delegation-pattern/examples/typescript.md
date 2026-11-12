```typescript run id=window-shape
interface Titled {
  readonly title: string;
}

interface Shape {
  area(): number;
  describe(owner: Titled): string;
}

class Rectangle implements Shape {
  private width: number;
  private height: number;
  constructor(width: number, height: number) {
    this.width = width;
    this.height = height;
  }
  area(): number {
    return this.width * this.height;
  }
  describe(owner: Titled): string {
    return `${owner.title}: a rectangle of ${this.width} by ${this.height}`;
  }
}

class Triangle implements Shape {
  private base: number;
  private height: number;
  constructor(base: number, height: number) {
    this.base = base;
    this.height = height;
  }
  area(): number {
    return Math.floor((this.base * this.height) / 2);
  }
  describe(owner: Titled): string {
    return `${owner.title}: a triangle of ${this.base} by ${this.height}`;
  }
}

class AppWindow implements Titled {
  readonly title: string;
  shape: Shape; // the delegate
  constructor(title: string, shape: Shape) {
    this.title = title;
    this.shape = shape;
  }
  area(): number {
    return this.shape.area(); // forward
  }
  describe(): string {
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

- Verdict: idiom. The delegate has the interface type `Shape`, so any class with the two methods can take its place.
- The delegate sees the receiver only through the small `Titled` interface, not the whole `AppWindow` class.

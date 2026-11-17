```typescript run id=structural-generic-overload
// Structural subtyping: a type fits Shape when it has the members.
interface Shape {
  area(): number;
}

class Square {
  side: number;
  constructor(side: number) {
    this.side = side;
  }
  area(): number {
    return this.side * this.side;
  }
}

const rect = { w: 2, h: 3, area() { return this.w * this.h; } };

function total(shapes: Shape[]): number {
  return shapes.reduce((sum, s) => sum + s.area(), 0);
}
console.log(total([new Square(2), rect]));

// Parametric polymorphism: one body for every element type.
function first<T>(items: T[]): T {
  return items[0];
}
const n: number = first([3, 4]);
const s: string = first(["a", "b"]);
console.log(n, s);

// Overload signatures: one implementation, a type for each call form.
function double(x: number): number;
function double(x: string): string;
function double(x: number | string): number | string {
  return typeof x === "number" ? x * 2 : x + x;
}
console.log(double(21), double("ab"));
```

```output for=structural-generic-overload
10
3 a
42 abab
```

- Verdict: native. `Square` and the object `rect` do not name `Shape`, but both have `area`, so both fit. `first` takes a type parameter `T`, and the checker gives `first([3, 4])` the type `number`. The two overload signatures give `double(21)` the type `number` and `double("ab")` the type `string`. The overloads have one body, which tests the type with `typeof`.

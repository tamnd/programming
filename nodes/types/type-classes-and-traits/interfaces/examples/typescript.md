```typescript run id=structural
interface Shape {
  readonly name: string;
  area(): number;
}

class Square implements Shape {
  readonly name = "square";
  side: number;
  constructor(side: number) {
    this.side = side;
  }
  area(): number {
    return this.side * this.side;
  }
}

class Circle {
  readonly name = "circle";
  radius: number;
  constructor(radius: number) {
    this.radius = radius;
  }
  area(): number {
    return Math.PI * this.radius * this.radius;
  }
}

function describe(shape: Shape): string {
  return `${shape.name} ${shape.area().toFixed(2)}`;
}

const triangle: Shape = { name: "triangle", area: () => 6 };

for (const shape of [new Square(2), new Circle(1), triangle]) {
  console.log(describe(shape));
}
```

```output
square 4.00
circle 3.14
triangle 6.00
```

```typescript run id=type-guard
interface Shape {
  area(): number;
}

function isShape(value: unknown): value is Shape {
  return typeof value === "object" && value !== null && typeof (value as Shape).area === "function";
}

const values: unknown[] = [{ area: () => 4 }, { side: 2 }, "square"];
for (const value of values) {
  console.log(isShape(value) ? value.area() : "not a shape");
}
```

```output
4
not a shape
not a shape
```

- Verdict: native. TypeScript compares interfaces by structure. `Circle` and the object literal have no `implements` clause, and `describe` accepts them.
- An `implements` clause only asks the compiler to check the class. It does not change the type of the class.
- The types are erased when the program runs. To test for an interface at run time, write a type predicate such as `isShape`, which checks the members.

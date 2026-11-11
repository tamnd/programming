```typescript run id=abstract
interface HasArea {
  area(): number;
}

abstract class Shape implements HasArea {
  protected readonly name: string;
  constructor(name: string) {
    this.name = name;
  }
  abstract area(): number;
  describe(): string {
    return `${this.name} with area ${this.area().toFixed(2)}`;
  }
}

class Rect extends Shape {
  private w: number;
  private h: number;
  constructor(w: number, h: number) {
    super("rect");
    this.w = w;
    this.h = h;
  }
  area(): number {
    return this.w * this.h;
  }
}

const unit: HasArea = { area: () => 1 };
const all: HasArea[] = [new Rect(2, 3), unit];
console.log(all.map((s) => s.area()));
console.log(new Rect(1.5, 2).describe());
```

```output for=abstract
[ 6, 1 ]
rect with area 3.00
```

- Verdict: native. TypeScript adds interfaces, abstract classes and the access modifiers `private`, `protected` and `public` to the class syntax of JavaScript.
- Interfaces are structural. The plain object `unit` has no class, but it has an `area` method, so it is a `HasArea`.
- The checks run at compile time only. At run time, `private` fields are ordinary properties. Use `#` fields for privacy that the engine enforces.

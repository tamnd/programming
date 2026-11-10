```typescript run id=shapes
type Point = readonly [number, number];

// The implementor: the primitive drawing operations of one output format.
interface Renderer {
  readonly name: string;
  circle(x: number, y: number, radius: number): string;
  polygon(points: readonly Point[]): string;
}

const textRenderer: Renderer = {
  name: "text",
  circle: (x, y, radius) => `circle at (${x}, ${y}) radius ${radius}`,
  polygon: (points) => "polygon " + points.map(([x, y]) => `(${x}, ${y})`).join(" "),
};

const svgRenderer: Renderer = {
  name: "svg",
  circle: (x, y, radius) => `<circle cx="${x}" cy="${y}" r="${radius}"/>`,
  polygon: (points) => `<polygon points="${points.map(([x, y]) => `${x},${y}`).join(" ")}"/>`,
};

// The abstraction: it holds a renderer and draws itself with the primitives.
abstract class Shape {
  renderer: Renderer;
  constructor(renderer: Renderer) {
    this.renderer = renderer;
  }
  abstract draw(): string;
  abstract scale(factor: number): void;
}

class Circle extends Shape {
  private readonly x: number;
  private readonly y: number;
  private radius: number;
  constructor(renderer: Renderer, x: number, y: number, radius: number) {
    super(renderer);
    this.x = x;
    this.y = y;
    this.radius = radius;
  }
  draw(): string {
    return this.renderer.circle(this.x, this.y, this.radius);
  }
  scale(factor: number): void {
    this.radius *= factor;
  }
}

class Square extends Shape {
  private readonly x: number;
  private readonly y: number;
  private side: number;
  constructor(renderer: Renderer, x: number, y: number, side: number) {
    super(renderer);
    this.x = x;
    this.y = y;
    this.side = side;
  }
  draw(): string {
    const { x, y, side: s } = this;
    return this.renderer.polygon([[x, y], [x + s, y], [x + s, y + s], [x, y + s]]);
  }
  scale(factor: number): void {
    this.side *= factor;
  }
}

for (const renderer of [textRenderer, svgRenderer]) {
  console.log(`${renderer.name}:`);
  for (const shape of [new Circle(renderer, 1, 1, 2), new Square(renderer, 0, 0, 3)]) {
    console.log(" ", shape.draw());
    shape.scale(2);
    console.log(" ", shape.draw());
  }
}

const circle = new Circle(textRenderer, 1, 1, 2);
circle.renderer = svgRenderer; // the implementation changes at run time
console.log("switched:", circle.draw());
```

```output for=shapes
text:
  circle at (1, 1) radius 2
  circle at (1, 1) radius 4
  polygon (0, 0) (3, 0) (3, 3) (0, 3)
  polygon (0, 0) (6, 0) (6, 6) (0, 6)
svg:
  <circle cx="1" cy="1" r="2"/>
  <circle cx="1" cy="1" r="4"/>
  <polygon points="0,0 3,0 3,3 0,3"/>
  <polygon points="0,0 6,0 6,6 0,6"/>
switched: <circle cx="1" cy="1" r="2"/>
```

- Verdict: idiom. An `interface` for the implementor and an `abstract class` for the abstraction. The compiler checks that each renderer has both methods with the right parameter types.
- The renderers are object literals, not classes. A structural type system accepts any object with the right shape, so the implementor side needs no class hierarchy.
- The fields are declared and assigned one by one. Constructor parameter properties such as `private x: number` would be shorter, but they are TypeScript syntax that emits code, and Node.js type stripping rejects them with `ERR_UNSUPPORTED_TYPESCRIPT_SYNTAX`.

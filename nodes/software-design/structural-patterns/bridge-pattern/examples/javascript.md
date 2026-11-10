```javascript run id=shapes
// The implementors: the primitive drawing operations of one output format.
class TextRenderer {
  name = "text";
  circle(x, y, radius) {
    return `circle at (${x}, ${y}) radius ${radius}`;
  }
  polygon(points) {
    return "polygon " + points.map(([x, y]) => `(${x}, ${y})`).join(" ");
  }
}

class SvgRenderer {
  name = "svg";
  circle(x, y, radius) {
    return `<circle cx="${x}" cy="${y}" r="${radius}"/>`;
  }
  polygon(points) {
    return `<polygon points="${points.map(([x, y]) => `${x},${y}`).join(" ")}"/>`;
  }
}

// The abstraction: it holds a renderer and draws itself with the primitives.
class Shape {
  constructor(renderer) {
    this.renderer = renderer;
  }
}

class Circle extends Shape {
  constructor(renderer, x, y, radius) {
    super(renderer);
    Object.assign(this, { x, y, radius });
  }
  draw() {
    return this.renderer.circle(this.x, this.y, this.radius);
  }
  scale(factor) {
    this.radius *= factor;
  }
}

class Square extends Shape {
  constructor(renderer, x, y, side) {
    super(renderer);
    Object.assign(this, { x, y, side });
  }
  draw() {
    const { x, y, side: s } = this;
    return this.renderer.polygon([[x, y], [x + s, y], [x + s, y + s], [x, y + s]]);
  }
  scale(factor) {
    this.side *= factor;
  }
}

for (const renderer of [new TextRenderer(), new SvgRenderer()]) {
  console.log(`${renderer.name}:`);
  for (const shape of [new Circle(renderer, 1, 1, 2), new Square(renderer, 0, 0, 3)]) {
    console.log(" ", shape.draw());
    shape.scale(2);
    console.log(" ", shape.draw());
  }
}

const circle = new Circle(new TextRenderer(), 1, 1, 2);
circle.renderer = new SvgRenderer(); // the implementation changes at run time
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

- Verdict: idiom. A shape holds a renderer object and calls its methods. JavaScript has no interfaces, so the two renderer classes share only the names of their methods.
- A renderer can be a plain object literal with `circle` and `polygon` functions. A class is not required for the implementor side.
- A mistake in a method name in one renderer shows up only when a shape calls it, as a `TypeError` at run time. The TypeScript example checks the methods at compile time.

```typescript run id=move
type Position = [number, number];

interface Movable {
  getPosition(): Position;
  setPosition(p: Position): void;
}

class MoveServant {
  moveTo(serviced: Movable, where: Position): void {
    serviced.setPosition(where);
  }
  moveBy(serviced: Movable, dx: number, dy: number): void {
    const [x, y] = serviced.getPosition();
    serviced.setPosition([x + dx, y + dy]);
  }
}

class Shape implements Movable {
  name: string;
  private position: Position;
  constructor(name: string, position: Position) {
    this.name = name;
    this.position = position;
  }
  getPosition(): Position { return this.position; }
  setPosition(p: Position): void { this.position = p; }
  toString(): string { return `${this.name} at (${this.position[0]}, ${this.position[1]})`; }
}
class Rectangle extends Shape {}
class Ellipse extends Shape {}
class Triangle extends Shape {}

const servant = new MoveServant();
const shapes: Shape[] = [new Rectangle("rectangle", [0, 0]), new Ellipse("ellipse", [10, 0]), new Triangle("triangle", [0, 10])];
for (const s of shapes) {
  servant.moveBy(s, 3, 4);
  console.log(String(s));
}
servant.moveTo(shapes[2], [1, 1]);
console.log(String(shapes[2]));
```

```output for=move
rectangle at (3, 4)
ellipse at (13, 4)
triangle at (3, 14)
triangle at (1, 1)
```

- Verdict: idiom. The interface `Movable` states what the servant needs.

```javascript run id=move
// The servant needs getPosition and setPosition from each served object.
class MoveServant {
  moveTo(serviced, where) {
    serviced.setPosition(where);
  }
  moveBy(serviced, dx, dy) {
    const [x, y] = serviced.getPosition();
    serviced.setPosition([x + dx, y + dy]);
  }
}

class Shape {
  #position;
  constructor(name, position) {
    this.name = name;
    this.#position = position;
  }
  getPosition() { return this.#position; }
  setPosition(p) { this.#position = p; }
  toString() { return `${this.name} at (${this.#position[0]}, ${this.#position[1]})`; }
}
class Rectangle extends Shape {}
class Ellipse extends Shape {}
class Triangle extends Shape {}

const servant = new MoveServant();
const shapes = [new Rectangle("rectangle", [0, 0]), new Ellipse("ellipse", [10, 0]), new Triangle("triangle", [0, 10])];
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

- Verdict: idiom. The interface is a convention: each served object has `getPosition` and `setPosition`.

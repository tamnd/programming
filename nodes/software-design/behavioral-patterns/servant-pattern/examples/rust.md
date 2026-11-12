```rust run id=move
use std::fmt;

#[derive(Clone, Copy)]
struct Position {
    x: i32,
    y: i32,
}

trait Movable {
    fn position(&self) -> Position;
    fn set_position(&mut self, p: Position);
}

struct MoveServant;

impl MoveServant {
    fn move_to(&self, m: &mut dyn Movable, where_: Position) {
        m.set_position(where_);
    }
    fn move_by(&self, m: &mut dyn Movable, dx: i32, dy: i32) {
        let p = m.position();
        m.set_position(Position { x: p.x + dx, y: p.y + dy });
    }
}

struct Shape {
    name: &'static str,
    pos: Position,
}

impl Movable for Shape {
    fn position(&self) -> Position {
        self.pos
    }
    fn set_position(&mut self, p: Position) {
        self.pos = p;
    }
}

impl fmt::Display for Shape {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{} at ({}, {})", self.name, self.pos.x, self.pos.y)
    }
}

fn main() {
    let servant = MoveServant;
    let mut shapes = [
        Shape { name: "rectangle", pos: Position { x: 0, y: 0 } },
        Shape { name: "ellipse", pos: Position { x: 10, y: 0 } },
        Shape { name: "triangle", pos: Position { x: 0, y: 10 } },
    ];
    for s in shapes.iter_mut() {
        servant.move_by(s, 3, 4);
        println!("{}", s);
    }
    servant.move_to(&mut shapes[2], Position { x: 1, y: 1 });
    println!("{}", shapes[2]);
}
```

```output for=move
rectangle at (3, 4)
ellipse at (13, 4)
triangle at (3, 14)
triangle at (1, 1)
```

- Verdict: idiom. A trait `Movable` states what the servant needs.

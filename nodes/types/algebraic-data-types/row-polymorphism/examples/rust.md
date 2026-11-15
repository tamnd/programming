```rust run id=keep
trait HasXY: Sized {
    fn x(&self) -> i32;
    fn y(&self) -> i32;
    fn with_x(self, x: i32) -> Self;
}

struct Point {
    x: i32,
    y: i32,
}

struct Labelled {
    x: i32,
    y: i32,
    label: String,
}

impl HasXY for Point {
    fn x(&self) -> i32 {
        self.x
    }
    fn y(&self) -> i32 {
        self.y
    }
    fn with_x(self, x: i32) -> Self {
        Point { x, ..self }
    }
}

impl HasXY for Labelled {
    fn x(&self) -> i32 {
        self.x
    }
    fn y(&self) -> i32 {
        self.y
    }
    fn with_x(self, x: i32) -> Self {
        Labelled { x, ..self }
    }
}

fn shift<P: HasXY>(p: P) -> P {
    let x = p.x();
    p.with_x(x + 1)
}

fn main() {
    let a = shift(Point { x: 1, y: 2 });
    let b = shift(Labelled { x: 1, y: 2, label: String::from("home") });
    println!("point: x={} y={}", a.x, a.y);
    println!("labelled: x={} y={} label={}", b.x, b.y, b.label);
}
```

```output for=keep
point: x=2 y=2
labelled: x=2 y=2 label=home
```

- Verdict: idiom. Traits cannot require fields, only methods, so `HasXY` has accessor methods. The bound `P: HasXY` keeps the full type of the argument in the result.
- The struct update syntax `Labelled { x, ..self }` copies the other fields, so `with_x` does not list them.

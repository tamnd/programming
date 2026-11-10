```rust run id=shapes
use std::rc::Rc;

/// The implementor: the primitive drawing operations of one output format.
trait Renderer {
    fn name(&self) -> &str;
    fn circle(&self, x: i32, y: i32, radius: i32) -> String;
    fn polygon(&self, points: &[(i32, i32)]) -> String;
}

struct TextRenderer;

impl Renderer for TextRenderer {
    fn name(&self) -> &str {
        "text"
    }
    fn circle(&self, x: i32, y: i32, radius: i32) -> String {
        format!("circle at ({x}, {y}) radius {radius}")
    }
    fn polygon(&self, points: &[(i32, i32)]) -> String {
        let parts: Vec<String> = points.iter().map(|(x, y)| format!("({x}, {y})")).collect();
        format!("polygon {}", parts.join(" "))
    }
}

struct SvgRenderer;

impl Renderer for SvgRenderer {
    fn name(&self) -> &str {
        "svg"
    }
    fn circle(&self, x: i32, y: i32, radius: i32) -> String {
        format!(r#"<circle cx="{x}" cy="{y}" r="{radius}"/>"#)
    }
    fn polygon(&self, points: &[(i32, i32)]) -> String {
        let parts: Vec<String> = points.iter().map(|(x, y)| format!("{x},{y}")).collect();
        format!(r#"<polygon points="{}"/>"#, parts.join(" "))
    }
}

/// The abstraction. Each shape holds a shared pointer to a renderer.
trait Shape {
    fn draw(&self) -> String;
    fn scale(&mut self, factor: i32);
}

struct Circle {
    renderer: Rc<dyn Renderer>,
    x: i32,
    y: i32,
    radius: i32,
}

impl Shape for Circle {
    fn draw(&self) -> String {
        self.renderer.circle(self.x, self.y, self.radius)
    }
    fn scale(&mut self, factor: i32) {
        self.radius *= factor;
    }
}

struct Square {
    renderer: Rc<dyn Renderer>,
    x: i32,
    y: i32,
    side: i32,
}

impl Shape for Square {
    fn draw(&self) -> String {
        let (x, y, s) = (self.x, self.y, self.side);
        self.renderer.polygon(&[(x, y), (x + s, y), (x + s, y + s), (x, y + s)])
    }
    fn scale(&mut self, factor: i32) {
        self.side *= factor;
    }
}

fn main() {
    let renderers: Vec<Rc<dyn Renderer>> = vec![Rc::new(TextRenderer), Rc::new(SvgRenderer)];
    for renderer in &renderers {
        println!("{}:", renderer.name());
        let mut shapes: Vec<Box<dyn Shape>> = vec![
            Box::new(Circle { renderer: Rc::clone(renderer), x: 1, y: 1, radius: 2 }),
            Box::new(Square { renderer: Rc::clone(renderer), x: 0, y: 0, side: 3 }),
        ];
        for shape in shapes.iter_mut() {
            println!("  {}", shape.draw());
            shape.scale(2);
            println!("  {}", shape.draw());
        }
    }

    let mut circle = Circle { renderer: Rc::new(TextRenderer), x: 1, y: 1, radius: 2 };
    circle.renderer = Rc::new(SvgRenderer); // the implementation changes at run time
    println!("switched: {}", circle.draw());
}
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

- Verdict: idiom. Two traits and a field of type `Rc<dyn Renderer>`. Many shapes share one renderer through the reference count.
- A generic field, `struct Circle<R: Renderer> { renderer: R }`, is the static form. The compiler then makes a separate `Circle` for each renderer type, and a call needs no dynamic dispatch. The price is that the renderer of a shape cannot change at run time, and shapes with different renderers cannot go in one `Vec` without a trait object.
- The renderers have no fields, so they are zero-sized types. `Rc::new(TextRenderer)` still allocates the reference counts.

```rust run id=window-shape
trait Titled {
    fn title(&self) -> &str;
}

trait Shape {
    fn area(&self) -> i32;
    fn describe(&self, owner: &dyn Titled) -> String;
}

struct Rectangle {
    width: i32,
    height: i32,
}

impl Shape for Rectangle {
    fn area(&self) -> i32 {
        self.width * self.height
    }
    fn describe(&self, owner: &dyn Titled) -> String {
        format!("{}: a rectangle of {} by {}", owner.title(), self.width, self.height)
    }
}

struct Triangle {
    base: i32,
    height: i32,
}

impl Shape for Triangle {
    fn area(&self) -> i32 {
        self.base * self.height / 2
    }
    fn describe(&self, owner: &dyn Titled) -> String {
        format!("{}: a triangle of {} by {}", owner.title(), self.base, self.height)
    }
}

struct AppWindow {
    title: String,
    shape: Box<dyn Shape>, // the delegate
}

impl Titled for AppWindow {
    fn title(&self) -> &str {
        &self.title
    }
}

impl AppWindow {
    fn area(&self) -> i32 {
        self.shape.area() // forward
    }
    fn describe(&self) -> String {
        self.shape.describe(self) // delegate, and pass the receiver
    }
}

fn main() {
    let mut w = AppWindow { title: "main".to_string(), shape: Box::new(Rectangle { width: 3, height: 4 }) };
    println!("area: {}", w.area());
    println!("{}", w.describe());
    w.shape = Box::new(Triangle { base: 3, height: 4 }); // change the behaviour at run time
    println!("area: {}", w.area());
    println!("{}", w.describe());
}
```

```output
area: 12
main: a rectangle of 3 by 4
area: 6
main: a triangle of 3 by 4
```

- Verdict: idiom. Rust has no inheritance and no forwarding keyword, so the window writes each forwarding method. A `Box<dyn Shape>` lets the delegate change at run time.

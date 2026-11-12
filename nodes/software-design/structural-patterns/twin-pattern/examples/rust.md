```rust run id=twin-ball
trait Item {
    fn name(&self) -> &str;
    fn position(&self) -> i32;
    fn show(&self) {
        println!("{} at {}", self.name(), self.position());
    }
}

trait Ticker {
    fn count_mut(&mut self) -> &mut u32;
    fn on_tick(&mut self);
    fn tick(&mut self) {
        *self.count_mut() += 1;
        self.on_tick();
    }
}

struct Ball {
    name: String,
    x: i32,
    count: u32,
}

impl Item for Ball {
    fn name(&self) -> &str {
        &self.name
    }
    fn position(&self) -> i32 {
        self.x
    }
}

impl Ticker for Ball {
    fn count_mut(&mut self) -> &mut u32 {
        &mut self.count
    }
    fn on_tick(&mut self) {
        self.x += 2;
    }
}

fn main() {
    let mut ball = Ball { name: "ball".to_string(), x: 0, count: 0 };
    ball.show();
    for _ in 0..2 {
        ball.tick();
        ball.show();
    }
    println!("ticks: {}", ball.count);
}
```

```output
ball at 0
ball at 2
ball at 4
ticks: 2
```

- Verdict: idiom. Rust has no inheritance. One type implements several traits, and each trait can give default methods, so the code of both parents goes to one type without a twin.

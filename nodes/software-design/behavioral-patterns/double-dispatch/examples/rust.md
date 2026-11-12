```rust run id=collisions
trait Body {
    fn name(&self) -> &'static str;
    fn collide(&self, other: &dyn Body) -> &'static str;
    fn collide_with_ship(&self) -> &'static str;
    fn collide_with_asteroid(&self) -> &'static str;
}

struct Ship;
struct Asteroid;

impl Body for Ship {
    fn name(&self) -> &'static str { "ship" }
    fn collide(&self, other: &dyn Body) -> &'static str { other.collide_with_ship() }
    fn collide_with_ship(&self) -> &'static str { "ships bounce" }
    fn collide_with_asteroid(&self) -> &'static str { "ship takes damage" }
}

impl Body for Asteroid {
    fn name(&self) -> &'static str { "asteroid" }
    fn collide(&self, other: &dyn Body) -> &'static str { other.collide_with_asteroid() }
    fn collide_with_ship(&self) -> &'static str { "ship takes damage" }
    fn collide_with_asteroid(&self) -> &'static str { "asteroids break apart" }
}

fn main() {
    let pairs: [(&dyn Body, &dyn Body); 4] =
        [(&Ship, &Asteroid), (&Asteroid, &Ship), (&Asteroid, &Asteroid), (&Ship, &Ship)];
    for (a, b) in pairs {
        println!("{} + {}: {}", a.name(), b.name(), a.collide(b));
    }
}
```

```output
ship + asteroid: ship takes damage
asteroid + ship: ship takes damage
asteroid + asteroid: asteroids break apart
ship + ship: ships bounce
```

- Verdict: idiom. Trait objects dispatch on the receiver, so `collide` calls back the other trait object. Another form, for a closed set of types, is an enum and one `match` on the pair `(a, b)`.

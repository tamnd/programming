```rust run id=light
#[derive(Debug, Clone, Copy, PartialEq)]
enum Light {
    Red,
    Amber,
    Green,
}

impl Light {
    const ALL: [Light; 3] = [Light::Red, Light::Amber, Light::Green];

    fn name(self) -> &'static str {
        match self {
            Light::Red => "red",
            Light::Amber => "amber",
            Light::Green => "green",
        }
    }

    fn after(self) -> Light {
        match self {
            Light::Red => Light::Green,
            Light::Green => Light::Amber,
            Light::Amber => Light::Red,
        }
    }
}

fn main() {
    for light in Light::ALL {
        println!("{} {} then {}", light.name(), light as i32, light.after().name());
    }
    println!("debug: {:?}", Light::Amber);
}
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
debug: Amber
```

- Verdict: native. An `enum` whose variants carry no data is an enumeration. `as i32` gives the number of a variant.
- A `match` must cover every variant, so `name` and `after` need no default branch.
- Rust cannot list the variants of an enum, so the type declares an `ALL` array.

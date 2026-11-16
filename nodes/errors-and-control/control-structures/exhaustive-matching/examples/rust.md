```rust run id=exhaustive-matching-rust
#[derive(Debug, Clone, Copy)]
enum Light {
    Red,
    Yellow,
    Green,
}

fn action(light: Light) -> &'static str {
    // No wildcard arm: a new variant makes this match fail to compile.
    match light {
        Light::Red => "stop",
        Light::Yellow => "slow down",
        Light::Green => "go",
    }
}

fn main() {
    for light in [Light::Red, Light::Yellow, Light::Green] {
        println!("{}: {}", format!("{light:?}").to_lowercase(), action(light));
    }
}
```

```output for=exhaustive-matching-rust
red: stop
yellow: slow down
green: go
```

- Verdict: native. A `match` must cover each value of its type, or the program does not compile. The match lists the three variants and has no wildcard arm, so the compiler reports each match that a new variant makes incomplete.

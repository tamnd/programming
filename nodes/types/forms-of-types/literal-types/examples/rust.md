```rust run id=enum-const-generic
#[derive(Debug, Clone, Copy, PartialEq)]
enum Align {
    Left,
    Right,
    Center,
}

impl std::str::FromStr for Align {
    type Err = String;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "left" => Ok(Align::Left),
            "right" => Ok(Align::Right),
            "center" => Ok(Align::Center),
            _ => Err(format!("bad align: {s}")),
        }
    }
}

// The modulus is part of the type, so Mod<5> and Mod<7> are different types.
#[derive(Debug, Clone, Copy)]
struct Mod<const N: u32>(u32);

impl<const N: u32> Mod<N> {
    fn add(self, other: Mod<N>) -> Mod<N> {
        Mod((self.0 + other.0) % N)
    }
}

fn main() {
    println!("{:?} {:?}", "center".parse::<Align>(), "centre".parse::<Align>());
    let a: Mod<5> = Mod(3);
    let b: Mod<5> = Mod(4);
    println!("{:?}", a.add(b));
    // let c: Mod<7> = Mod(1); a.add(c); // error: expected Mod<5>, found Mod<7>
    println!("{}", std::mem::size_of::<Mod<5>>());
}
```

```output for=enum-const-generic
Ok(Center) Err("bad align: centre")
Mod(2)
4
```

- Verdict: idiom. Rust has no literal types. An `enum` with unit variants is a closed set, and code that reads text must parse it into a variant. A const generic parameter puts a value in a type: `Mod<5>` and `Mod<7>` are different types, so the compiler rejects adding numbers with different moduli. The modulus costs no memory in each value.

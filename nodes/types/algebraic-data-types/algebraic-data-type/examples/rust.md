```rust run id=count
#[derive(Clone, Copy)]
enum Light {
    Red,
    Amber,
    Green,
}

const LIGHTS: [Light; 3] = [Light::Red, Light::Amber, Light::Green];

// Product: a flag AND a light.
struct Pair {
    flag: bool,
    light: Light,
}

// Sum: a flag OR a light.
enum Choice {
    Flag(bool),
    Light(Light),
}

fn name(l: Light) -> &'static str {
    match l {
        Light::Red => "red",
        Light::Amber => "amber",
        Light::Green => "green",
    }
}

fn yn(b: bool) -> &'static str {
    if b { "yes" } else { "no" }
}

fn show(c: &Choice) -> String {
    match c {
        Choice::Flag(f) => format!("flag:{}", yn(*f)),
        Choice::Light(l) => format!("light:{}", name(*l)),
    }
}

fn main() {
    let flags = [false, true];
    let mut pairs = Vec::new();
    for &flag in &flags {
        for &light in &LIGHTS {
            pairs.push(Pair { flag, light });
        }
    }
    let mut choices: Vec<Choice> = flags.iter().map(|&f| Choice::Flag(f)).collect();
    choices.extend(LIGHTS.iter().map(|&l| Choice::Light(l)));

    println!("pairs: {}", pairs.len());
    let p: Vec<String> = pairs.iter().map(|p| format!("{}/{}", yn(p.flag), name(p.light))).collect();
    println!("{}", p.join(" "));
    println!("choices: {}", choices.len());
    let c: Vec<String> = choices.iter().map(show).collect();
    println!("{}", c.join(" "));
}
```

```output for=count
pairs: 6
no/red no/amber no/green yes/red yes/amber yes/green
choices: 5
flag:no flag:yes light:red light:amber light:green
```

- Verdict: native. A `struct` is a product, and an `enum` whose variants carry data is a sum.
- A `match` over `Choice` must cover both variants, or the program does not compile.
- `Light` is an `enum` whose variants carry no data, so it is an enumeration.

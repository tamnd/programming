```rust run id=enum-and-match
#[derive(Clone, Copy, PartialEq, Debug)]
enum S {
    Start,
    Sign,
    Int,
    Dot,
    Frac,
    Dead,
}

fn step(s: S, ch: char) -> S {
    let digit = ch.is_ascii_digit();
    match (s, ch) {
        (S::Start, '+' | '-') => S::Sign,
        (S::Start | S::Sign | S::Int, _) if digit => S::Int,
        (S::Int, '.') => S::Dot,
        (S::Dot | S::Frac, _) if digit => S::Frac,
        _ => S::Dead,
    }
}

fn accepts(text: &str) -> bool {
    let end = text.chars().fold(S::Start, step);
    matches!(end, S::Int | S::Frac)
}

fn main() {
    for t in ["42", "-3.5", "1.", "+", "007", ".5", "4.2.1"] {
        println!("{t} {}", accepts(t));
    }
}
```

```output for=enum-and-match
42 true
-3.5 true
1. false
+ false
007 true
.5 false
4.2.1 false
```

```rust fails expect=error:E0004 id=missing-state
#[derive(Clone, Copy)]
enum S {
    Start,
    Sign,
    Int,
    Dot,
    Frac,
    Dead,
}

fn is_final(s: S) -> bool {
    match s { // the state Dead has no arm
        S::Int | S::Frac => true,
        S::Start | S::Sign | S::Dot => false,
    }
}

fn main() {
    println!("{}", is_final(S::Dead));
}
```

- Verdict: idiom. A Rust `enum` names the states, and a `match` on the pair of state and character gives the transitions. `fold` runs the machine over the characters of the input. Libraries such as `regex` build automata from patterns.
- The fails block has a `match` with no arm for the state `Dead`. Rust requires a `match` to cover every value, so adding a state to the enum makes the compiler point at every `match` that does not handle it.

```rust run id=patterns
#[derive(Debug)]
enum Cmd<'a> {
    Get(&'a str),
    Set(&'a str, &'a str),
    Quit,
}

fn parse<'a>(words: &[&'a str]) -> Option<Cmd<'a>> {
    match words {
        ["get", key] => Some(Cmd::Get(*key)),
        ["set", key, value] => Some(Cmd::Set(*key, *value)),
        ["quit"] | ["exit"] => Some(Cmd::Quit),
        _ => None,
    }
}

fn main() {
    for line in ["get name", "set name ada", "exit", "set name"] {
        let words: Vec<&str> = line.split_whitespace().collect();
        println!("{:?}", parse(&words));
    }
}
```

```output for=patterns
Some(Get("name"))
Some(Set("name", "ada"))
Some(Quit)
None
```

- Verdict: idiom. Rust code is imperative, but `match` with slice patterns states the shape of each valid input, and the compiler builds the tests. Derive attributes such as `#[derive(Debug)]` declare traits that the compiler then writes.
- The standard library has no regular expressions. The `regex` crate is the common choice.

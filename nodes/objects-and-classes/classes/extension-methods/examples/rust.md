```rust run id=extension-trait
trait WordCount {
    fn word_count(&self) -> usize;
}

impl WordCount for str {
    // a local trait, implemented for a type of the standard library
    fn word_count(&self) -> usize {
        self.split_whitespace().count()
    }
}

fn main() {
    let s = "Hello extension traits";
    println!("{} {}", s.word_count(), WordCount::word_count("one two"));
    println!("{}", String::from("a b c").word_count()); // a String derefs to str
}
```

```output for=extension-trait
3 2
3
```

- Verdict: idiom. Rust has no extension methods, but a crate can define a trait and implement it for a type from the standard library, such as `str`. The orphan rule allows this because the trait is local.
- The method is in scope only where the trait is in scope, which is the same rule as an import of C# extension methods. `WordCount::word_count("one two")` calls it as a plain function.

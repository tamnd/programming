```rust run id=decompose
use std::collections::HashMap;

fn split_words(text: &str) -> Vec<String> {
    text.split_whitespace().map(|w| w.to_lowercase()).collect()
}

fn count_words(words: &[String]) -> HashMap<&str, usize> {
    let mut counts = HashMap::new();
    for w in words {
        *counts.entry(w.as_str()).or_insert(0) += 1;
    }
    counts
}

fn top<'a>(counts: &HashMap<&'a str, usize>, n: usize) -> Vec<(&'a str, usize)> {
    let mut all: Vec<_> = counts.iter().map(|(w, c)| (*w, *c)).collect();
    all.sort_by(|a, b| b.1.cmp(&a.1).then(a.0.cmp(b.0)));
    all.truncate(n);
    all
}

fn report(text: &str, n: usize) {
    let words = split_words(text);
    for (word, c) in top(&count_words(&words), n) {
        println!("{word:<5} {c}");
    }
}

fn main() {
    report("The cat and the hat and the bat", 2);
}
```

```output for=decompose
the   3
and   2
```

- Verdict: native. Free functions in modules are the base of a Rust program, and `pub` marks the ones that other modules can call. The signatures show which arguments a function borrows and which it may change.

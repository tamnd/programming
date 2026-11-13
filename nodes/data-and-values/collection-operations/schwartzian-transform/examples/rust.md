```rust run id=sort-by-cached-key
use std::cell::Cell;

fn main() {
    let words = vec!["banana", "kiwi", "apple", "fig", "cherry", "date", "plum", "mango"];
    let calls = Cell::new(0);
    let vowels = |w: &str| {
        calls.set(calls.get() + 1);
        w.chars().filter(|c| "aeiou".contains(*c)).count()
    };

    // sort_by_key calls the key function at each comparison.
    let mut slow = words.clone();
    slow.sort_by_key(|w| vowels(w));
    println!("{:?} {}", slow, calls.get() > words.len());

    // sort_by_cached_key calls it at most once for each item.
    calls.set(0);
    let mut cached = words.clone();
    cached.sort_by_cached_key(|w| vowels(w));
    println!("{:?} {}", cached, calls.get());

    // The same steps by hand: decorate, sort, undecorate.
    calls.set(0);
    let mut d: Vec<(usize, &str)> = words.iter().map(|w| (vowels(w), *w)).collect();
    d.sort_by_key(|p| p.0);
    let fast: Vec<&str> = d.into_iter().map(|p| p.1).collect();
    println!("{} {}", fast == cached, calls.get());
}
```

```output
["fig", "cherry", "plum", "kiwi", "apple", "date", "mango", "banana"] true
["fig", "cherry", "plum", "kiwi", "apple", "date", "mango", "banana"] 8
true 8
```

`sort_by_key` calls the key function at each comparison. `sort_by_cached_key` calls it once for each item and gives the same order. A vector of `(key, word)` pairs sorted with `sort_by_key` on the stored key does the same steps by hand, with 8 calls.

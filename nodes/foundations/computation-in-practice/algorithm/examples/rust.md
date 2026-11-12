```rust run id=binary-search-result
fn main() {
    let mut xs = vec![90, 12, 58, 31, 47];
    xs.sort_unstable(); // faster, and equal items may change order
    println!("sorted: {:?}", xs);

    for t in [58, 50] {
        match xs.binary_search(&t) {
            Ok(i) => println!("search {t}: found at {i}"),
            Err(i) => println!("search {t}: not found, insert at {i}"),
        }
    }

    // partition_point: the first index where the predicate turns false
    let i = xs.partition_point(|&x| x < 40);
    println!("first item >= 40 is at {i}: {}", xs[i]);

    let mut people = vec![("Grace", 1906), ("Ada", 1815), ("Alan", 1912)];
    people.sort_by_key(|p| p.1); // stable
    println!("{:?}", people);
}
```

```output for=binary-search-result
sorted: [12, 31, 47, 58, 90]
search 58: found at 3
search 50: not found, insert at 3
first item >= 40 is at 2: 47
[("Ada", 1815), ("Grace", 1906), ("Alan", 1912)]
```

- Verdict: stdlib. Slices have a stable `sort`, a faster `sort_unstable` and `sort_by_key`. `binary_search` returns `Ok(index)` when the value is there and `Err(index)` with the insertion point when it is not, so the type makes the caller handle both cases. `partition_point` gives the first index where a predicate turns false, which is a lower bound for any condition.

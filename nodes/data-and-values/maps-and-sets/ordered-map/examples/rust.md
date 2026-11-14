```rust run id=btreemap
use std::collections::BTreeMap;
use std::ops::Bound::{Excluded, Included, Unbounded};

fn main() {
    // A BTreeMap keeps its keys sorted.
    let mut m = BTreeMap::new();
    for (k, v) in [(30, "c"), (10, "a"), (50, "e"), (20, "b"), (40, "d")] {
        m.insert(k, v);
    }
    println!("{:?}", m);
    println!("{:?} {:?}", m.first_key_value(), m.last_key_value());

    // Floor and ceiling: a range and one step from the right end or the left end.
    let floor = m.range(..=35).next_back();
    let ceiling = m.range(35..).next();
    println!("{:?} {:?}", floor, ceiling);
    println!("{:?}", m.range(..5).next_back());

    // A range of keys, with each bound included or excluded.
    let part: Vec<_> = m.range(20..45).map(|(k, _)| *k).collect();
    println!("{:?}", part);
    let open: Vec<_> = m.range((Excluded(20), Unbounded)).map(|(k, _)| *k).collect();
    let closed: Vec<_> = m.range((Included(20), Included(40))).map(|(k, _)| *k).collect();
    println!("{:?} {:?}", open, closed);

    // pop_first removes the smallest entry, and split_off cuts the map in two.
    println!("{:?}", m.pop_first());
    let high = m.split_off(&40);
    println!("{:?} {:?}", m, high);

    // Keys need Ord. f64 has no total order because of NaN, so BTreeMap<f64, _> does not compile.
    // An integer key, or a wrapper type with a total order, is the usual fix.
    let mut by_cents: BTreeMap<i64, &str> = BTreeMap::new();
    by_cents.insert(199, "tea");
    by_cents.insert(99, "gum");
    println!("{:?}", by_cents);
}
```

```output
{10: "a", 20: "b", 30: "c", 40: "d", 50: "e"}
Some((10, "a")) Some((50, "e"))
Some((30, "c")) Some((40, "d"))
None
[20, 30, 40]
[30, 40, 50] [20, 30, 40]
Some((10, "a"))
{20: "b", 30: "c"} {40: "d", 50: "e"}
{99: "gum", 199: "tea"}
```

- Verdict: stdlib. BTreeMap keeps its keys sorted. first_key_value and last_key_value give the ends, range with next and next_back gives floor and ceiling, and Bound sets each end of a range. pop_first and split_off change the map. Keys need Ord, so f64 is not a key.

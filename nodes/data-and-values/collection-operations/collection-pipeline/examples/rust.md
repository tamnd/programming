```rust run id=iterator-chain
use std::cell::Cell;
use std::collections::BTreeMap;

struct Article {
    title: &'static str,
    words: u32,
    tags: &'static [&'static str],
}

fn main() {
    let articles = [
        Article { title: "Intro to NoSQL", words: 1200, tags: &["nosql", "databases"] },
        Article { title: "Event sourcing", words: 800, tags: &["architecture"] },
        Article { title: "Graph stores", words: 1500, tags: &["nosql", "graphs"] },
        Article { title: "Refactoring loops", words: 600, tags: &["refactoring"] },
        Article { title: "Key-value stores", words: 900, tags: &["nosql"] },
    ];

    // filter and collect, then sort, take and map.
    let mut nosql: Vec<&Article> = articles.iter().filter(|a| a.tags.contains(&"nosql")).collect();
    nosql.sort_by(|a, b| b.words.cmp(&a.words));
    let top: Vec<&str> = nosql.iter().take(2).map(|a| a.title).collect();
    println!("{:?}", top);

    // map and fold: fold is the reduce of Rust iterators.
    let total = nosql.iter().map(|a| a.words).fold(0, |acc, n| acc + n);
    println!("{}", total);

    // flat_map and a fold into a map: the number of articles for each tag.
    let counts = articles.iter().flat_map(|a| a.tags.iter()).fold(BTreeMap::new(), |mut m, t| {
        *m.entry(*t).or_insert(0) += 1;
        m
    });
    println!("{:?}", counts);

    let calls = Cell::new(0);
    let score = |a: &Article| {
        calls.set(calls.get() + 1);
        a.words
    };

    // A Vec between the steps makes the pipeline eager: every score is computed.
    let all: Vec<u32> = articles.iter().map(score).collect();
    let eager: Vec<u32> = all.into_iter().filter(|s| *s > 1000).take(1).collect();
    println!("eager {:?} {}", eager, calls.get());

    // One iterator chain is lazy: take(1) stops the work after the first match.
    calls.set(0);
    let lazy: Vec<u32> = articles.iter().map(score).filter(|s| *s > 1000).take(1).collect();
    println!("lazy {:?} {}", lazy, calls.get());
}
```

```output
["Graph stores", "Intro to NoSQL"]
3600
{"architecture": 1, "databases": 1, "graphs": 1, "nosql": 3, "refactoring": 1}
eager [1200] 5
lazy [1200] 1
```

- Verdict: stdlib. The iterator adapters filter, map, take and flat_map are lazy, and collect, fold or a for loop runs them. A Cell holds the call counter, so the closure score can change it and still be copied into two chains. collect into a Vec between map and filter makes the pipeline eager, so score runs 5 times. The chain without the Vec stops after the first match.

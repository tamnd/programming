```rust run id=graph
use std::collections::BTreeMap;

fn main() {
    let edges = [("a", "b"), ("a", "c"), ("b", "c"), ("c", "d"), ("d", "a")];

    let mut graph: BTreeMap<&str, Vec<&str>> = BTreeMap::new();
    for (source, target) in edges {
        graph.entry(source).or_default().push(target);
        graph.entry(target).or_default();
    }

    for (node, neighbors) in &graph {
        println!("{node} -> {}", neighbors.join(" "));
    }

    let has_edge = |x: &str, y: &str| graph.get(x).is_some_and(|n| n.contains(&y));
    let yes_no = |b: bool| if b { "yes" } else { "no" };
    println!("edge a->c: {}", yes_no(has_edge("a", "c")));
    println!("edge c->a: {}", yes_no(has_edge("c", "a")));

    let in_degree: usize = graph
        .values()
        .map(|n| n.iter().filter(|t| **t == "c").count())
        .sum();
    let edge_count: usize = graph.values().map(Vec::len).sum();
    println!("in-degree of c: {in_degree}");
    println!("edges: {edge_count}");
}
```

```output for=graph
a -> b c
b -> c
c -> d
d -> a
edge a->c: yes
edge c->a: no
in-degree of c: 2
edges: 5
```

- Verdict: idiom. A `BTreeMap` from each node to a `Vec` of neighbors is an adjacency list, with the keys in sorted order.
- `entry(...).or_default()` creates an empty list the first time a node appears.

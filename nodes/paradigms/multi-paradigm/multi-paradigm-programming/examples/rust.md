```rust run id=orders
use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq)]
struct Order {                                  // a value type
    customer: String,
    amount: i32,
}

fn parse(line: &str) -> Order {                 // a pure function
    let (name, amount) = line.split_once(',').expect("a comma");
    Order { customer: name.trim().to_string(), amount: amount.trim().parse().expect("a number") }
}

trait Report {                                  // one interface, two formats
    fn line(&self, name: &str, total: i32) -> String;
}
struct Plain;
struct Csv;
impl Report for Plain {
    fn line(&self, n: &str, t: i32) -> String { format!("{n} {t}") }
}
impl Report for Csv {
    fn line(&self, n: &str, t: i32) -> String { format!("{n},{t}") }
}

fn main() {
    let text = "ana, 30\nbo, 70\nana, 25\ncy, 10\nbo, 5";
    let mut totals: HashMap<String, i32> = HashMap::new();
    for o in text.lines().map(parse) {          // a loop that mutates a map
        *totals.entry(o.customer).or_insert(0) += o.amount;
    }
    let mut big: Vec<(String, i32)> = totals.into_iter().filter(|&(_, t)| t > 50).collect();
    big.sort_by_key(|&(_, t)| std::cmp::Reverse(t));
    let reports: Vec<Box<dyn Report>> = vec![Box::new(Plain), Box::new(Csv)];
    for r in &reports {
        for (name, total) in &big {
            println!("{}", r.line(name, *total));
        }
    }
}
```

```output for=orders
bo 75
ana 55
bo,75
ana,55
```

- Verdict: native. Rust mixes the imperative style, with mutable variables and loops, and the functional style, with algebraic data types, pattern matching, closures and iterators. Traits give interfaces with static dispatch through generics and dynamic dispatch through `dyn`, but there is no inheritance of data. The borrow checker makes mutation safe, so the loop that updates `totals` needs no copies.

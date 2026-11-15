```rust run id=loop-with-let-else-and-a-join
fn main() {
    // loop with the exit in the middle: read, test, process.
    let mut input = "7\n2\nstop\n9".lines();
    let mut sum = 0;
    loop {
        let Some(line) = input.next() else { break };
        if line == "stop" {
            break;
        }
        sum += line.parse::<i32>().unwrap();
    }
    println!("sum: {sum}");
    println!("left: {:?}", input.collect::<Vec<_>>());

    // Separators between the items: take the first item out of the loop.
    let items = ["a", "b", "c"];
    let mut out = String::new();
    let mut it = items.iter();
    if let Some(first) = it.next() {
        out.push_str(first);
        for item in it {
            out.push_str(", ");
            out.push_str(item);
        }
    }
    println!("{out}");
}
```

```output for=loop-with-let-else-and-a-join
sum: 9
left: ["9"]
a, b, c
```

- Verdict: idiom. `loop` has two exits in the middle: `let ... else { break }` at the end of the input and `break` at the stop word, so `9` stays unread. The join takes the first item out of the loop, so each pass of the `for` adds a separator and then an item. `items.join(", ")` does the same in one call.

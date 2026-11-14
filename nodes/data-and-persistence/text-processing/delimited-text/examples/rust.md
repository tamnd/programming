```rust run id=split-fields
fn main() {
    // split keeps every empty field, also the last one.
    let v: Vec<&str> = "a::b:".split(':').collect();
    println!("{:?}", v);
    let v: Vec<&str> = "".split(':').collect();
    println!("{:?}", v);

    // split_whitespace splits at runs of white space and drops the ends.
    let v: Vec<&str> = "  a  b ".split_whitespace().collect();
    println!("{:?}", v);

    // splitn(2, ..) keeps the rest of the line in the last field.
    let v: Vec<&str> = "k=v=w".splitn(2, '=').collect();
    println!("{:?}", v);
}
```

```output for=split-fields
["a", "", "b", ""]
[""]
["a", "b"]
["k", "v=w"]
```

- Verdict: stdlib. `str::split` keeps each empty field, and `split_whitespace` splits at runs of white space. `splitn(2, ..)` keeps the rest of the line in the last field. The standard library has no CSV reader.

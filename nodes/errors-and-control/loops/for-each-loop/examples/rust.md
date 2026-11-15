```rust run id=borrow-change-and-move
fn main() {
    let mut v = vec![1, 2, 3];

    // &v borrows each element.
    let mut sum = 0;
    for x in &v {
        sum += x;
    }
    println!("sum: {sum}");

    // &mut v borrows each element for change.
    for x in &mut v {
        *x *= 10;
    }
    println!("{v:?}");

    for (i, x) in v.iter().enumerate() {
        println!("{i}: {x}");
    }

    // v by value moves the elements into the loop; v cannot be used after it.
    let words = vec![String::from("a"), String::from("b")];
    for w in words {
        println!("owned {w}");
    }
}
```

```output for=borrow-change-and-move
sum: 6
[10, 20, 30]
0: 10
1: 20
2: 30
owned a
owned b
```

- Verdict: native. A `for` loop takes anything that implements `IntoIterator`. `&v` gives shared references, `&mut v` gives mutable references that the body changes through `*x`, and `words` by value moves each `String` into `w`, so a use of `words` after the loop would not compile.

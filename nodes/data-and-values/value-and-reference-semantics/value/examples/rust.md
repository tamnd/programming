```rust run id=place-and-value-expressions
fn main() {
    // A value expression gives a value; a place expression names a location.
    let mut x = 5;
    x = x + 1;
    let r = &x;
    println!("{} {}", x, *r);

    // Moving a String transfers the value; the old place can no longer be read.
    let s = String::from("text");
    let t = s;
    println!("{t}");

    // Copy types are copied instead: both places keep a value.
    let a = 7;
    let b = a;
    println!("{a} {b}");

    // A temporary value lives until the end of the statement.
    let len = String::from("temporary").len();
    println!("{len}");

    // A field and an index are place expressions too.
    let mut point = (1, 2);
    point.0 = 10;
    let mut arr = [0; 3];
    arr[1] = 4;
    println!("{:?} {:?}", point, arr);

    // Functions and closures are values.
    let ops: [fn(i32) -> i32; 2] = [|v| v + 1, |v| v * 10];
    println!("{:?}", ops.iter().map(|f| f(3)).collect::<Vec<_>>());
}
```

```output for=place-and-value-expressions
6 6
text
7 7
9
(10, 2) [0, 4, 0]
[4, 30]
```

- Verdict: native. `x` on the left of `=` and in `&x` is a place expression. `let t = s` moves the `String`, so `s` cannot be used after it, but an integer is `Copy`, so `a` and `b` both keep a value. A temporary `String` lives until the end of its statement. A tuple field and an array index are place expressions too. Function pointers are values in an array.

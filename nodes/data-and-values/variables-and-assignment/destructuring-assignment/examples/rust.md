```rust run id=patterns
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let (q, r) = (17 / 5, 17 % 5);
    println!("{q} {r}");

    let p = Point { x: 3, y: 4 };
    let Point { x, y: height } = p;
    println!("{x} {height}");

    let items = [1, 2, 3, 4, 5];
    let [first, .., last] = items;
    println!("{first} {last}");

    let pairs = vec![("a", 1), ("b", 2)];
    for (key, value) in &pairs {
        println!("{key}={value}");
    }

    let (mut a, mut b) = (1, 2);
    (a, b) = (b, a);
    println!("{a} {b}");
}
```

```output for=patterns
3 2
3 4
1 5
a=1
b=2
2 1
```

- Verdict: native. A `let` takes a pattern, so tuples, structs and arrays are taken apart where they are declared. `y: height` binds the field `y` to the name `height`, and `..` skips the other items. The same patterns work in `for`, `match` and parameters, and an assignment can also destructure into existing variables.

```rust run id=copy-move-clone
#[derive(Debug, Clone, Copy)]
struct Point {
    x: i32,
}

fn main() {
    let a = Point { x: 1 };
    let mut b = a; // Point is Copy, so this copies the bits
    b.x = 9;
    println!("{:?} {:?}", a, b);

    let s = String::from("hi");
    let t = s; // String is not Copy, so this moves it
    // println!("{}", s); // error: s was moved
    println!("{}", t);

    let mut u = t.clone(); // clone makes a second string
    u.push('!');
    println!("{} {}", t, u);

    let mut v = vec![1, 2];
    let r = &mut v; // a reference shares explicitly
    r.push(3);
    println!("{:?}", v);
}
```

```output for=copy-move-clone
Point { x: 1 } Point { x: 9 }
hi
hi hi!
[1, 2, 3]
```

- Verdict: idiom. Every Rust type is a value type. `Point` implements `Copy`, so assignment copies it. `String` owns heap memory and is not `Copy`, so assignment moves it, and the compiler rejects a later use of `s`. `clone` makes an independent copy. A reference such as `&mut v` is the explicit way to share.

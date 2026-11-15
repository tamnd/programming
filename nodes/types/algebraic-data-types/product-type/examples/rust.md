```rust run id=parts
#[derive(Debug, PartialEq)]
struct Point {
    x: i32,
    y: i32,
}

fn yn(b: bool) -> &'static str {
    if b { "yes" } else { "no" }
}

fn main() {
    let t = (3, 4); // parts by position
    let (x, y) = t;
    println!("tuple: {t:?} x = {x} y = {y} t.0 = {}", t.0);

    let p = Point { x: 3, y: 4 }; // parts by name
    println!("record: {p:?} x = {} y = {}", p.x, p.y);
    println!("same parts equal: {}", yn(p == Point { x: 3, y: 4 }));
    println!("swapped parts equal: {}", yn(p == Point { x: 4, y: 3 }));

    let unit = ();
    println!("unit: {unit:?} size: {}", std::mem::size_of::<()>());
}
```

```output for=parts
tuple: (3, 4) x = 3 y = 4 t.0 = 3
record: Point { x: 3, y: 4 } x = 3 y = 4
same parts equal: yes
swapped parts equal: no
unit: () size: 0
```

- Verdict: native. A tuple is a product by position, and a struct is a product by name.
- `#[derive(PartialEq)]` makes `==` compare the fields one by one.
- The unit type `()` has one value and a size of 0 bytes.

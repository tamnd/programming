```rust run id=tuple-struct-enum
#[derive(Debug, Clone, PartialEq)]
struct Person {
    name: String,
    age: u32,
}

#[derive(Debug)]
enum Shape {
    Circle(f64),
    Rect { w: f64, h: f64 },
}

#[derive(Debug)]
enum List {
    Cons(i32, Box<List>),
    Nil,
}

fn main() {
    let t: (i32, &str) = (1, "a");
    let arr: [u8; 3] = [1, 2, 3];
    println!("{:?} {} {:?} {}", t, t.1, arr, std::mem::size_of::<[u8; 3]>());

    let p = Person { name: String::from("Ann"), age: 31 };
    let q = p.clone();
    println!("{:?} {}", q, p == q);

    let shapes = [Shape::Circle(1.0), Shape::Rect { w: 2.0, h: 3.0 }];
    for s in &shapes {
        let area = match s {
            Shape::Circle(r) => 3.0 * r * r,
            Shape::Rect { w, h } => w * h,
        };
        println!("{:?} {}", s, area);
    }

    // A recursive type needs a pointer such as Box, so its size is known.
    let list = List::Cons(1, Box::new(List::Cons(2, Box::new(List::Nil))));
    println!("{:?}", list);
}
```

```output for=tuple-struct-enum
(1, "a") a [1, 2, 3] 3
Person { name: "Ann", age: 31 } true
Circle(1.0) 3
Rect { w: 2.0, h: 3.0 } 6
Cons(1, Cons(2, Nil))
```

- Verdict: native. A tuple finds its parts by position, such as `t.1`, and an array of 3 bytes has a size of 3 bytes. A `struct` gets `==` and a copy only when it derives `PartialEq` and `Clone`. An `enum` holds one of its variants, each with its own data, and `match` takes it apart. The recursive `List` holds the next node in a `Box`, because a type that contains itself directly would have no finite size.

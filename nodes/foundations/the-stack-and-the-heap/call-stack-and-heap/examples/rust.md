```rust run id=box-and-vec
use std::mem::size_of;

#[allow(dead_code)]
enum List {
    Cons(i32, Box<List>), // Box puts the rest of the list on the heap
    Nil,
}

fn sum(list: &List) -> i32 {
    let mut total = 0;
    let mut node = list;
    while let List::Cons(v, next) = node {
        total += v;
        node = next;
    }
    total
}

fn make() -> Vec<i32> {
    let v = vec![1, 2, 3]; // the header is in the frame, the elements on the heap
    v // moving v out copies the header, not the elements
}

fn main() {
    let a = [0u8; 1024]; // 1024 bytes in the frame
    let b = Box::new([0u8; 1024]); // one pointer in the frame
    println!(
        "{} {}",
        size_of::<[u8; 1024]>(),
        size_of::<Box<[u8; 1024]>>()
    );
    println!("{} {}", a.len(), b.len());

    let v = make();
    println!("{:?} {}", v, size_of::<Vec<i32>>() / size_of::<usize>());

    let mut list = List::Nil;
    for i in 1..=4 {
        list = List::Cons(i, Box::new(list));
    }
    println!("{}", sum(&list));
}
```

```output for=box-and-vec
1024 8
1024 1024
[1, 2, 3] 3
10
```

```rust fails expect=error:E0515 id=return-reference-to-local
fn largest(values: &[i32]) -> &i32 {
    let mut best = values[0];
    for &v in values {
        if v > best {
            best = v;
        }
    }
    &best // best lives in this frame
}

fn main() {
    println!("{}", largest(&[3, 9, 4]));
}
```

- Verdict: native. Rust values live in the frame by default, and the program puts data on the heap through types that own a heap allocation: `Box` for one value, `Vec` and `String` for a buffer that grows. An array of 1024 bytes takes 1024 bytes in the frame, while a `Box` of it takes one pointer, and a `Vec` takes three words: a pointer, a length and a capacity. The heap memory is freed when its owner goes out of scope.
- A recursive type such as a linked list needs a `Box`, because a value that contains itself directly would have no finite size.
- The fails block returns a reference to a local variable. The variable dies with the frame, so the borrow checker rejects the program with E0515. Return the value itself, or a reference into the argument.

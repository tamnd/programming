```rust run id=largest
fn largest<T: PartialOrd + Copy>(items: &[T]) -> T {
    let mut result = items[0];
    for &item in items {
        if item > result {
            result = item;
        }
    }
    result
}

struct Stack<T> {
    items: Vec<T>,
}

impl<T> Stack<T> {
    fn push(&mut self, item: T) {
        self.items.push(item);
    }
}

fn main() {
    println!("{}", largest(&[3, 9, 4]));
    println!("{}", largest(&["pear", "apple", "plum"]));
    let mut s = Stack { items: Vec::new() };
    s.push(2.5);
    println!("{}", s.items.len());
}
```

```output for=largest
9
plum
1
```

- Verdict: native. Functions, structs, enums, traits and `impl` blocks take type parameters.
- A trait bound such as `T: PartialOrd + Copy` states what the body may do with `T`. The compiler checks the body against the bounds, once, before any call.
- The compiler monomorphizes generic code: it makes one copy for each concrete type that the program uses. A call costs the same as a call of a function written for that type.
- A trait object (`&dyn Trait` or `Box<dyn Trait>`) is the alternative with one copy of the code and dynamic dispatch.

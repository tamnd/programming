```rust run id=change
fn set_copy(mut n: i32) {
    n = 42;
    println!("inside set_copy: {n}");
}

fn set_through(r: &mut i32) {
    *r = 42;
}

fn main() {
    let mut x = 1;
    set_copy(x);
    println!("after set_copy: {x}");
    set_through(&mut x);
    println!("after set_through: {x}");

    let numbers = vec![1, 2, 3];
    let a = &numbers;
    let b = &numbers;
    println!("a and b are the same vector: {}", std::ptr::eq(a, b));
    let copy = numbers.clone();
    println!("copy is the same vector: {}", std::ptr::eq(&copy, &numbers));
    println!("copy == numbers: {}", copy == numbers);

    let p = &raw mut x;
    unsafe {
        *p += 1;
    }
    println!("after raw pointer write: {x}");
}
```

```output for=change
inside set_copy: 42
after set_copy: 1
after set_through: 42
a and b are the same vector: true
copy is the same vector: false
copy == numbers: true
after raw pointer write: 43
```

- Verdict: native. A shared reference `&T` and a mutable reference `&mut T` point to a value that the compiler proves is alive.
- The borrow checker allows any number of shared references or one mutable reference at a time. A shared reference stops changes to the value.
- `std::ptr::eq` compares two references by address. `==` on references compares the values.
- The raw pointers `*const T` and `*mut T` have no such guarantees. A dereference of a raw pointer is valid only inside an `unsafe` block.
- `&raw mut x` makes a raw pointer directly, with no reference in between.

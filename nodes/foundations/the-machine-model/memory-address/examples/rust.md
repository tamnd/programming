```rust run id=raw-pointers-and-layout
use std::mem::{align_of, offset_of, size_of};

#[allow(dead_code)]
#[repr(C)]
struct Rec {
    flag: u8,
    count: i32,
    tag: u8,
}

#[allow(dead_code)]
struct Reordered {
    flag: u8,
    count: i32,
    tag: u8,
}

fn main() {
    let xs: [i32; 4] = [10, 20, 30, 40];
    let p0: *const i32 = &xs[0];
    let p2: *const i32 = &xs[2];
    println!("distance: {} bytes", p2.addr() - p0.addr());
    // Reading through a raw pointer needs unsafe: the compiler cannot check it.
    let v = unsafe { *p0.add(3) };
    println!("value at p0 + 3 elements: {v}");

    println!("repr(C) offsets: {} {} {}", offset_of!(Rec, flag), offset_of!(Rec, count), offset_of!(Rec, tag));
    println!("repr(C) size {} align {}", size_of::<Rec>(), align_of::<Rec>());
    println!("default layout size {}", size_of::<Reordered>()); // Rust may reorder fields
}
```

```output for=raw-pointers-and-layout
distance: 8 bytes
value at p0 + 3 elements: 40
repr(C) offsets: 0 4 8
repr(C) size 12 align 4
default layout size 8
```

- Verdict: native. A reference converts to a raw pointer without `unsafe`, and `addr` gives its address as a `usize`, but reading through a raw pointer needs an `unsafe` block, because the compiler cannot check that `p0.add(3)` stays in the array. `offset_of!` gives the offsets of a `repr(C)` struct, which has the C layout. Without `repr(C)`, the compiler may reorder the fields, and today it puts the two bytes together after the `i32`, so the struct takes 8 bytes and not 12.

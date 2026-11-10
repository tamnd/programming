```rust fails expect=panic id=overflow
use std::hint::black_box;

fn main() {
    let x: i32 = black_box(i32::MAX);
    println!("{}", x + 1);
}
```

Integer overflow is not undefined behavior in Rust. In a debug build, as here, the addition panics with "attempt to add with overflow". In a release build without overflow checks, it wraps. `black_box` hides the value from the compiler, so that the compiler does not reject the constant overflow at compile time.

```rust run id=explicit
fn main() {
    let x = i32::MAX;
    println!("{:?}", x.checked_add(1));
    println!("{}", x.wrapping_add(1));
    println!("{}", x.saturating_add(1));
    println!("{:?}", x.overflowing_add(1));
    let v = vec![1, 2, 3];
    println!("{:?}", v.get(3));
}
```

```output for=explicit
None
-2147483648
2147483647
(-2147483648, true)
None
```

```rust fails expect=compile id=raw-pointer
fn main() {
    let x = 5;
    let p = &raw const x;
    println!("{}", *p);
}
```

Dereferencing a raw pointer is an unsafe operation. Outside an `unsafe` block, the compiler rejects it with error E0133.

```rust run id=unsafe-block
fn main() {
    let x = 5;
    let p = &raw const x;
    // SAFETY: p points to x, which is alive and initialized here.
    let y = unsafe { *p };
    println!("{y}");
}
```

```output for=unsafe-block
5
```

- Verdict: native. Undefined behavior exists in Rust, but only code in an `unsafe` block or an `unsafe fn` can cause it.
- In safe code, overflow panics or wraps, an index out of range panics, and `get` returns `None`. None of these is undefined behavior.
- The `unsafe` keyword does not turn off the checks of safe code. It allows a short list of extra operations, such as the dereference of a raw pointer, and the programmer must make sure that each use is valid.
- A `// SAFETY:` comment that says why the operation is valid is the usual style for each `unsafe` block.

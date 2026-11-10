```rust run id=const-fn
const fn factorial(n: u64) -> u64 {
    let mut r = 1;
    let mut i = 2;
    while i <= n {
        r *= i;
        i += 1;
    }
    r
}

const F10: u64 = factorial(10);
const _: () = assert!(factorial(5) == 120);

fn main() {
    let slots = [0u8; factorial(4) as usize];
    println!("F10 = {F10}");
    println!("slots: {}", slots.len());
    let n = std::env::args().count() as u64 + 5;
    println!("factorial({n}) = {}", factorial(n));
}
```

```output for=const-fn
F10 = 3628800
slots: 24
factorial(6) = 720
```

The initializer of a `const` item and the length of an array are const contexts, and the compiler evaluates them. `assert!` in a `const` item runs at compile time. The call with `n` is outside a const context, so it is an ordinary call.

```rust fails expect=compile id=assert
const fn factorial(n: u64) -> u64 {
    let mut r = 1;
    let mut i = 2;
    while i <= n {
        r *= i;
        i += 1;
    }
    r
}

const _: () = assert!(factorial(5) == 100, "5! is 120, not 100");

fn main() {
    println!("unreachable");
}
```

A panic during constant evaluation is a compile error. So `assert!` in an unnamed `const` item is the Rust form of a static assertion.

```rust fails expect=compile id=overflow
const fn factorial(n: u32) -> u32 {
    let mut r = 1;
    let mut i = 2;
    while i <= n {
        r *= i;
        i += 1;
    }
    r
}

const F13: u32 = factorial(13);

fn main() {
    println!("{F13}");
}
```

13! does not fit in `u32`. In a const context, overflow is a compile error (E0080), and the compiler shows the call that failed.

```rust fails expect=compile id=for-loop
const fn sum(xs: &[u32]) -> u32 {
    let mut total = 0;
    for x in xs {
        total += *x;
    }
    total
}

fn main() {
    println!("{}", sum(&[1, 2, 3]));
}
```

A `for` loop calls trait methods of `IntoIterator` and `Iterator`. They are not const functions, so the compiler rejects the loop with error E0015. A `while` loop with an index compiles.

- Verdict: native. `const fn` functions run at compile time in const contexts: `const` and `static` initializers, array lengths, enum discriminants, const generic arguments and `const` blocks.
- The set of operations allowed in a `const fn` is smaller than in a normal function, and it has grown since Rust 1.31.

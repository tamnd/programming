```rust run id=const-evaluation
const fn fib(n: u32) -> u64 {
    let (mut a, mut b) = (0, 1);
    let mut i = 0;
    while i < n {
        (a, b) = (b, a + b);
        i += 1;
    }
    a
}

const FIB_50: u64 = fib(50); // computed by the compiler, stored in the binary

const TABLE: [u64; 8] = {
    let mut t = [0; 8];
    let mut i = 0;
    while i < 8 {
        t[i] = fib(i as u32);
        i += 1;
    }
    t
};

fn main() {
    println!("{FIB_50}");
    println!("{TABLE:?}");
    let n: u32 = std::env::args().count() as u32 + 9; // known only at run time
    println!("{}", fib(n)); // the same function, called at run time
}
```

```output for=const-evaluation
12586269025
[0, 1, 1, 2, 3, 5, 8, 13]
55
```

```rust fails expect=error:E0080 id=overflow-at-compile-time
const fn fib(n: u32) -> u8 {
    let (mut a, mut b) = (0u8, 1u8);
    let mut i = 0;
    while i < n {
        (a, b) = (b, a + b);
        i += 1;
    }
    a
}

const BIG: u8 = fib(20); // the compiler runs fib and finds the overflow

fn main() {
    println!("{BIG}");
}
```

- Verdict: native. `rustc` compiles ahead of time, through LLVM, to a machine-code executable that runs without Rust installed. A `const fn` can run inside the compiler: `FIB_50` and `TABLE` are computed during compilation and stored in the binary, and the same `fib` also runs at run time with an argument that the compiler cannot know.
- The fails block computes a `const` whose evaluation overflows a `u8`. The compiler runs the function, finds the overflow, and rejects the program with E0080, so the error never reaches a user.

```rust run id=gcd-with-exit-code
fn gcd(mut a: u64, mut b: u64) -> u64 {
    while b != 0 {
        (a, b) = (b, a % b);
        println!("{a} {b}");
    }
    a
}

fn main() -> std::process::ExitCode {
    println!("1071 462");
    println!("gcd {}", gcd(1071, 462));
    std::process::ExitCode::SUCCESS
}
```

```output for=gcd-with-exit-code
1071 462
462 147
147 21
21 0
gcd 21
```

- Verdict: native. A Rust program is a crate whose root has a `main` function, and the compiler links the crate and its dependencies into one executable. `main` can return `ExitCode`, or a `Result` whose error gives a failing status. A tuple assignment updates both variables at once.

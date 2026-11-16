```rust run id=function-pointer-and-delegate-rust
type BinOp = fn(i32, i32) -> i32;

fn add(a: i32, b: i32) -> i32 {
    a + b
}

fn mul(a: i32, b: i32) -> i32 {
    a * b
}

fn apply(f: BinOp, a: i32, b: i32) -> i32 {
    f(a, b)
}

fn main() {
    let mut op: BinOp = add;
    println!("{}", op(6, 7));
    op = mul;
    println!("{}", op(6, 7));
    println!("{}", apply(|a, b| a - b, 6, 7));
}
```

```output for=function-pointer-and-delegate-rust
13
42
-1
```

- Verdict: native. `BinOp` is the function pointer type `fn(i32, i32) -> i32`. The closure `|a, b| a - b` captures nothing, so it coerces to that type.

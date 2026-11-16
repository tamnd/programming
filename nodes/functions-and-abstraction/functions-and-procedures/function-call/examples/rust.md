```rust run id=function-call-rust
fn rectangle_area(width: i32, height: i32) -> i32 {
    println!("computing {} by {}", width, height);
    width * height
}

fn main() {
    let area = rectangle_area(3, 4);
    println!("area {}", area);
}
```

```output for=function-call-rust
computing 3 by 4
area 12
```

- Verdict: native. A call expression is the function operand, then the arguments in parentheses.

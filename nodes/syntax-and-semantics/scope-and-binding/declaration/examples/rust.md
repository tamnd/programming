```rust run id=items-and-extern-blocks
// Items can be in any order, so main can use helper and Point.
fn main() {
    println!("helper() = {}", helper());
    let p = Point { x: 1, y: 2 };
    println!("p = ({}, {})", p.x, p.y);

    // A let declaration without a value; the compiler checks each path.
    let y;
    if p.x > 0 { y = "positive"; } else { y = "other"; }
    println!("y = {y}");

    // An item inside a block is visible in the whole block.
    println!("inner() = {}", inner());
    fn inner() -> i32 { 7 }

    // A declaration of a foreign function; the C library gives the definition.
    let a = unsafe { abs(-3) };
    println!("abs(-3) = {a}");
}

unsafe extern "C" {
    fn abs(x: i32) -> i32;
}

fn helper() -> &'static str { "ok" }

struct Point { x: i32, y: i32 }
```

```output for=items-and-extern-blocks
helper() = ok
p = (1, 2)
y = positive
inner() = 7
abs(-3) = 3
```

- Verdict: native. Items can come in any order, also inside a block, so `main` uses `helper`, `Point` and `inner` before their declarations. A `let` without a value is valid if each path assigns it. An `extern` block declares the C function `abs`, and the C library gives the definition, so the call needs `unsafe`.

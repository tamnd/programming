```rust run id=block-expressions-and-drop
struct Noisy(&'static str);

impl Drop for Noisy {
    fn drop(&mut self) { println!("drop {}", self.0); }
}

fn plus_one(x: i32) -> i32 { x + 1 }

fn main() {
    // A block is an expression: its value is the value of its last expression.
    let area = {
        let w = 3;
        let h = 4;
        w * h
    };
    println!("area = {area}");

    // A value is dropped at the end of the block that owns it.
    {
        let _a = Noisy("a");
        println!("in the block");
    }
    println!("after the block");

    // A labeled block can break with a value.
    let first = 'search: {
        for n in [3, 8, 12] {
            if n % 4 == 0 { break 'search n; }
        }
        0
    };
    println!("first multiple of 4 = {first}");

    // A semicolon after the last expression makes the value ().
    let unit = { plus_one(area); };
    println!("unit = {unit:?}");
}
```

```output for=block-expressions-and-drop
area = 12
in the block
drop a
after the block
first multiple of 4 = 8
unit = ()
```

- Verdict: native. A block is an expression, and its value is the value of its final operand. A value is dropped at the end of the block that owns it. A labeled block can `break` with a value. A semicolon after the last expression makes the value of the block `()`.

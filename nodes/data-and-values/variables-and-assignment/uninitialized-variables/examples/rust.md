```rust run id=checked-paths
use std::mem::MaybeUninit;

fn main() {
    let n = std::env::args().count();
    let size: &str;
    match n {
        0 => size = "none",
        1 => size = "one",
        _ => size = "many",
    }
    println!("{size}");

    let mut slot = MaybeUninit::<u32>::uninit();
    slot.write(7);
    let value = unsafe { slot.assume_init() };
    println!("{value}");
}
```

```output for=checked-paths
one
7
```

- Verdict: native. The compiler checks that each path assigns `size` before the read, and rejects the program if one arm of the `match` does not. The program name is the first argument, so `n` is 1. For memory that the program fills later, `MaybeUninit` holds a value that may not exist yet, and `assume_init` is unsafe: the programmer promises that the value is there.

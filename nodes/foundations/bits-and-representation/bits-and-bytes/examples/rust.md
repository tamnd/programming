```rust run id=checked-shifts-and-byte-order
const READ: u8 = 1;
const WRITE: u8 = 1 << 1;
const EXEC: u8 = 1 << 2;

fn main() {
    let mut perm = READ | WRITE;
    perm &= !WRITE; // ! is bitwise NOT on integers
    perm ^= EXEC;
    println!("{perm:03b} {} {}", perm.count_ones(), perm & EXEC != 0);

    let n: i32 = -8;
    println!("{} {} {}", n >> 1, (n as u32) >> 28, 0b1001_0110u8.rotate_left(1));
    println!("{:?} {:?} {}", 1u32.checked_shl(32), 1u32.wrapping_shl(33), 1u32.leading_zeros());

    let x: u32 = 0x12345678;
    let le = x.to_le_bytes();
    println!("{:02x?} {:x}", le, u32::from_be_bytes(le));
}
```

```output for=checked-shifts-and-byte-order
101 2 true
-4 15 45
None 2 31
[78, 56, 34, 12] 78563412
```

```rust fails expect=compile id=shift-past-width
fn main() {
    let mask: u8 = 1 << 8; // a u8 has bits 0 to 7 only
    println!("{mask}");
}
```

- Verdict: native. Rust has `&`, `|`, `^`, `<<`, `>>` and `!`, which is bitwise NOT on integers. A right shift is arithmetic for signed types and logical for unsigned ones. A shift count at or above the width is overflow: it panics in a debug build, and `checked_shl` and `wrapping_shl` choose another result explicitly. Integer types have `count_ones`, `leading_zeros`, `rotate_left`, `to_le_bytes`, `from_be_bytes` and other bit methods.
- The fails block shifts a `u8` by 8 in a constant expression. The compiler sees the overflow and rejects the program with the `arithmetic_overflow` lint, which is an error by default.

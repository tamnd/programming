```rust run id=forms
fn to_sign_magnitude(n: i32) -> u32 {
    if n < 0 { 0x80 | (-n) as u32 } else { n as u32 }
}

fn to_ones(n: i32) -> u32 {
    if n < 0 { !((-n) as u32) & 0xFF } else { n as u32 }
}

fn from_sign_magnitude(b: u32) -> i32 {
    if b & 0x80 != 0 { -((b & 0x7F) as i32) } else { b as i32 }
}

fn from_ones(b: u32) -> i32 {
    if b & 0x80 != 0 { -((!b & 0xFF) as i32) } else { b as i32 }
}

fn yes_no(b: bool) -> &'static str {
    if b { "yes" } else { "no" }
}

fn main() {
    println!("-43 in sign-magnitude: {:08b}", to_sign_magnitude(-43));
    println!("-43 in ones' complement: {:08b}", to_ones(-43));
    println!("10101011 in sign-magnitude: {}", from_sign_magnitude(0b10101011));
    println!("11010100 in ones' complement: {}", from_ones(0b11010100));
    println!("10000000 in sign-magnitude: {}", from_sign_magnitude(0b10000000));
    println!("11111111 in ones' complement: {}", from_ones(0b11111111));

    let z = -0.0_f64;
    println!("-0.0 == 0.0: {}", yes_no(z == 0.0));
    println!("(-0.0).is_sign_negative(): {}", yes_no(z.is_sign_negative()));
    println!("1.0 / -0.0: {}", 1.0 / z);
}
```

```output for=forms
-43 in sign-magnitude: 10101011
-43 in ones' complement: 11010100
10101011 in sign-magnitude: -43
11010100 in ones' complement: -43
10000000 in sign-magnitude: 0
11111111 in ones' complement: 0
-0.0 == 0.0: yes
(-0.0).is_sign_negative(): yes
1.0 / -0.0: -inf
```

- Verdict: idiom. Rust's signed integers are two's complement, so a program makes the other two forms by hand. Rust writes bitwise NOT as `!`.
- `is_sign_negative` reads the sign bit of a float, so it finds the sign of `-0.0`.

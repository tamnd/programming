```rust run id=inline-assembly
use std::arch::asm;

// Inline assembly puts machine instructions into a Rust function.
// The compiler picks the registers and fills in {0} and {1}.
fn add(mut a: u64, b: u64) -> u64 {
    unsafe {
        #[cfg(target_arch = "aarch64")]
        asm!("add {0}, {0}, {1}", inout(reg) a, in(reg) b);
        #[cfg(target_arch = "x86_64")]
        asm!("add {0}, {1}", inout(reg) a, in(reg) b);
    }
    a
}

fn leading_zeros(x: u64) -> u64 {
    let n: u64;
    unsafe {
        #[cfg(target_arch = "aarch64")]
        asm!("clz {0}, {1}", out(reg) n, in(reg) x);
        #[cfg(target_arch = "x86_64")]
        asm!("bsr {0}, {1}", "xor {0}, 63", out(reg) n, in(reg) x); // x must not be 0
    }
    n
}

fn main() {
    println!("add(40, 2) = {}", add(40, 2));
    println!("add(u64::MAX, 1) = {}", add(u64::MAX, 1)); // the machine add wraps around
    println!("leading zeros of 1: {}", leading_zeros(1));
    println!("same as the library: {}", leading_zeros(1 << 20) == u64::from((1u64 << 20).leading_zeros()));
}
```

```output for=inline-assembly
add(40, 2) = 42
add(u64::MAX, 1) = 0
leading zeros of 1: 63
same as the library: true
```

- Verdict: native. `asm!` puts instructions inside a Rust function, with one version for each processor, chosen by `cfg`. The operands say which values go in and out, and the compiler chooses the registers. The machine add wraps around, unlike the `+` of Rust in a debug build, which panics on overflow. The leading-zero count uses `clz` on ARM64 and `bsr` on x86-64, and gives the same result as the library function.

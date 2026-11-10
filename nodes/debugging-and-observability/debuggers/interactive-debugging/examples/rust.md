```rust run id=backtrace
use std::backtrace::{Backtrace, BacktraceStatus};

/// Returns the names of the functions of this program on the stack,
/// innermost first, without addresses and file paths.
fn where_am_i() -> Vec<String> {
    let trace = Backtrace::force_capture(); // ignores RUST_BACKTRACE
    trace
        .to_string()
        .lines()
        .filter_map(|line| line.trim().split_once(": ").map(|(_, name)| name.to_string()))
        .filter(|name| name.starts_with("main::"))
        .collect()
}

#[inline(never)]
fn price(item: &str) -> u32 {
    if item == "bread" {
        println!("  {}", where_am_i().join(" <- "));
        return 250;
    }
    30
}

#[inline(never)]
fn total(cart: &[&str]) -> u32 {
    cart.iter().map(|item| price(item)).sum()
}

fn main() {
    println!("total: {}", total(&["apple", "bread"]));
    let status = Backtrace::force_capture().status();
    println!("captured: {}", status == BacktraceStatus::Captured);
}
```

```output for=backtrace
  main::where_am_i <- main::price <- main::total::{closure#0} <- main::total <- main::main
total: 280
captured: true
```

`Backtrace::capture` reads the environment first. When `RUST_BACKTRACE` and `RUST_LIB_BACKTRACE` are not set, it records nothing.

```rust wrong id=capture
use std::backtrace::Backtrace;

fn price(item: &str) -> u32 {
    if item == "bread" {
        // capture() reads RUST_BACKTRACE and RUST_LIB_BACKTRACE. When neither
        // is set, it captures nothing, and the trace is empty.
        let trace = Backtrace::capture();
        println!("status: {:?}", trace.status());
        println!("trace: {trace}");
        return 250;
    }
    30
}

fn main() {
    let total: u32 = ["apple", "bread"].iter().map(|item| price(item)).sum();
    println!("total: {total}");
}
```

```output for=capture
status: Disabled
trace: disabled backtrace
total: 280
```

The program asked for a trace and printed an empty one. Use `Backtrace::force_capture` when the trace must be there, or set `RUST_BACKTRACE=1`.

- Verdict: library. The Rust standard library has no debugger. A native debugger such as gdb or lldb runs the program, and `std::backtrace` prints the stack from the code.
- `rustc -g` is the same as `-C debuginfo=2`: full debug information for the debugger. Without a flag, rustc writes no debug information.
- The names in a trace depend on the optimization level. The closure in `total` has a frame of its own only when the compiler does not inline it.

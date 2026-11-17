```rust run id=trace
use std::backtrace::Backtrace;

/// The names of the functions of this program on the stack, innermost first.
#[inline(never)]
fn where_am_i() -> String {
    let trace = Backtrace::force_capture(); // ignores RUST_BACKTRACE
    let names: Vec<String> = trace
        .to_string()
        .lines()
        .filter_map(|line| line.trim().split_once(": ").map(|(_, name)| name.to_string()))
        .filter_map(|name| name.strip_prefix("main::").map(str::to_string))
        .filter(|name| name != "where_am_i")
        .collect();
    names.join(" <- ")
}

#[inline(never)]
fn c() {
    println!("      stack: {}", where_am_i());
}

#[inline(never)]
fn b() {
    println!("    call c");
    c();
    println!("    back in b");
}

#[inline(never)]
fn a() {
    println!("  call b");
    b();
    println!("  back in a");
}

fn main() {
    println!("call a");
    a();
    println!("back in main");
}
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. `Backtrace::force_capture` records the stack of the current thread. The text of a backtrace is for people, and its format can change, so the code keeps only the names of this program.
- `#[inline(never)]` keeps the compiler from inlining the functions, so each one has its own frame.

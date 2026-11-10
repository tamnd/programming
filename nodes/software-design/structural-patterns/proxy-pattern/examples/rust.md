```rust run id=report
use std::cell::OnceCell;

trait Report {
    fn render(&self) -> String;
}

struct DiskReport {
    orders: u32,
}

impl DiskReport {
    fn load() -> Self {
        println!("load: read the report from disk");
        DiskReport { orders: 42 }
    }
}

impl Report for DiskReport {
    fn render(&self) -> String {
        format!("report: {} orders", self.orders)
    }
}

// A virtual proxy: the OnceCell is empty until the first call fills it.
struct LazyReport {
    real: OnceCell<DiskReport>,
}

impl Report for LazyReport {
    fn render(&self) -> String {
        self.real.get_or_init(DiskReport::load).render()
    }
}

// A protection proxy over any Report.
struct Guarded<'a, R: Report> {
    inner: &'a R,
    role: &'a str,
}

impl<R: Report> Report for Guarded<'_, R> {
    fn render(&self) -> String {
        if self.role != "admin" {
            return "access denied".to_string();
        }
        self.inner.render()
    }
}

fn main() {
    let report = LazyReport { real: OnceCell::new() };
    println!("proxy made");
    println!("{}", report.render());
    println!("{}", report.render());
    for role in ["guest", "admin"] {
        println!("{role}: {}", Guarded { inner: &report, role }.render());
    }
}
```

```output
proxy made
load: read the report from disk
report: 42 orders
report: 42 orders
guest: access denied
admin: report: 42 orders
```

- Verdict: stdlib. `OnceCell` holds a value that is set at most once. `get_or_init` runs the function only when the cell is empty, and it works through `&self`, so `render` does not need `&mut self`.
- `OnceCell` is not thread safe. `OnceLock` is the form for many threads, and `LazyCell` and `LazyLock` hold the function in the cell.
- `Guarded` is generic over `R`, so the compiler makes a version for each real type, and the call to `inner.render()` is direct, not through a vtable.
- Rust smart pointers are proxies in the sense of Design Patterns. `Deref` lets `Box`, `Rc` and `MutexGuard` forward method calls to the value they hold.

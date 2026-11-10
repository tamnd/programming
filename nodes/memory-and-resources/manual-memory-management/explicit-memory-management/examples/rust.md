```rust run id=alloc-dealloc
use std::alloc::{alloc, dealloc, handle_alloc_error, Layout};

struct Order {
    id: u32,
    quantity: u32,
}

fn main() {
    let layout = Layout::array::<u32>(4).expect("layout fits in isize");
    unsafe {
        let totals = alloc(layout) as *mut u32;
        if totals.is_null() {
            handle_alloc_error(layout);
        }
        for i in 0..4 {
            totals.add(i).write(i as u32 * 10);
        }
        println!("last total: {}", totals.add(3).read());
        dealloc(totals as *mut u8, layout); // the same layout as alloc
    }

    let order = Box::new(Order { id: 7, quantity: 2 });
    let raw: *mut Order = Box::into_raw(order); // the caller now owns the memory
    unsafe {
        println!("order {} has {} items", (*raw).id, (*raw).quantity);
        drop(Box::from_raw(raw)); // ownership goes back to a Box, which frees it
    }
}
```

```output
last total: 30
order 7 has 2 items
```

- Verdict: stdlib. Safe Rust frees memory through ownership; `std::alloc` gives unsafe code direct access to the global allocator.
- `dealloc` must receive the same `Layout` that `alloc` received. A null result from `alloc` means failure, and `handle_alloc_error` reports it.
- `Box::into_raw` gives up ownership without freeing the memory. `Box::from_raw` takes it back, and the `Box` frees the memory when it is dropped.
- Calling `Box::from_raw` twice on the same pointer frees the memory twice.

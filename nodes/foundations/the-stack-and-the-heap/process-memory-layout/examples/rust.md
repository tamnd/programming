```rust run id=regions
use std::fs;
use std::sync::atomic::AtomicI32;

static COUNTER: AtomicI32 = AtomicI32::new(42); // a global that can change: the data segment

// Prints the permissions and the kind of the region that holds addr.
fn where_is(name: &str, addr: usize) {
    let exe = fs::read_link("/proc/self/exe").unwrap();
    let exe = exe.to_string_lossy().into_owned();
    let maps = fs::read_to_string("/proc/self/maps").unwrap();
    for line in maps.lines() {
        let fields: Vec<&str> = line.splitn(6, ' ').collect();
        let path = fields.get(5).map_or("", |s| s.trim());
        let (lo, hi) = fields[0].split_once('-').unwrap();
        let start = usize::from_str_radix(lo, 16).unwrap();
        let end = usize::from_str_radix(hi, 16).unwrap();
        if addr >= start && addr < end {
            let kind = if path == exe {
                "the program file"
            } else if path.is_empty() || path.starts_with("[anon") {
                "an anonymous mapping"
            } else {
                path
            };
            println!("{:<8} {} {}", name, fields[1], kind);
            return;
        }
    }
}

fn main() {
    let here = 1i32;              // a local: the stack
    let small = Box::new(7i32);   // a small block: the heap
    let big = vec![1u8; 1 << 20]; // 1 MiB, over MMAP_THRESHOLD

    where_is("code", where_is as fn(&str, usize) as usize);
    where_is("global", &COUNTER as *const AtomicI32 as usize);
    where_is("local", &here as *const i32 as usize);
    where_is("small", &*small as *const i32 as usize);
    where_is("big", big.as_ptr() as usize);
}
```

```output for=regions
code     r-xp the program file
global   rw-p the program file
local    rw-p [stack]
small    rw-p [heap]
big      rw-p an anonymous mapping
```

- Verdict: native. A reference converts to a raw pointer, and the pointer to an address. `Box` and `Vec` use the system allocator by default, so the blocks go where `malloc` puts them.
- An immutable `static` with a plain value can be put in read-only data. `AtomicI32` can change, so `COUNTER` is in a region that can be written.

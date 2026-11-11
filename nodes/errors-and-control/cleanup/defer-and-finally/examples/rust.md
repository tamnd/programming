```rust run id=copy
struct File {
    name: String,
}

impl File {
    fn open(name: &str) -> File {
        println!("open {name}");
        File { name: name.to_string() }
    }
}

impl Drop for File {
    fn drop(&mut self) {
        println!("close {}", self.name);
    }
}

fn copy(src: &str, dst: &str, fail: bool) -> Result<(), String> {
    let a = File::open(src);
    let b = File::open(dst);
    if fail {
        return Err(format!("cannot read {}", a.name));
    }
    println!("copy {} to {}", a.name, b.name);
    Ok(())
}

fn main() {
    println!("result: {:?}", copy("a.txt", "b.txt", false));
    println!("result: {:?}", copy("a.txt", "c.txt", true));

    let result = std::panic::catch_unwind(|| {
        let _guard = File::open("d.txt");
        panic!("bug while d.txt is open");
    });
    println!("panicked: {}", result.is_err());

    let early = File::open("e.txt");
    drop(early);
    println!("e.txt is closed before the end of main");
}
```

```output
open a.txt
open b.txt
copy a.txt to b.txt
close b.txt
close a.txt
result: Ok(())
open a.txt
open c.txt
close c.txt
close a.txt
result: Err("cannot read a.txt")
open d.txt
close d.txt
panicked: true
open e.txt
close e.txt
e.txt is closed before the end of main
```

- Verdict: native. A type that implements `Drop` runs its `drop` method when its value goes out of scope. This covers a normal return, an early return with `?` or `return`, and unwinding after a panic.
- Variables are dropped in reverse order of declaration, so `b` closes before `a`.
- `drop(value)` ends the life of a value before the end of its scope.
- Rust has no `finally` and no `defer`. A guard value with `Drop` does that work.

The pattern `_` does not bind the value. The guard is a temporary, and it is dropped at the end of the statement, before the work that it should protect.

```rust wrong id=underscore
struct Lock(&'static str);

impl Drop for Lock {
    fn drop(&mut self) {
        println!("unlock {}", self.0);
    }
}

fn lock(name: &'static str) -> Lock {
    println!("lock {name}");
    Lock(name)
}

fn main() {
    let _ = lock("accounts");
    println!("update accounts");
    let _held = lock("orders");
    println!("update orders");
}
```

```output
lock accounts
unlock accounts
update accounts
lock orders
update orders
unlock orders
```

`std::mem::forget` takes ownership of a value and does not run its destructor. It is a safe function, so code must not depend on `drop` for memory safety.

```rust wrong id=forget
struct File(&'static str);

impl Drop for File {
    fn drop(&mut self) {
        println!("close {}", self.0);
    }
}

fn main() {
    let a = File("a.txt");
    let b = File("b.txt");
    std::mem::forget(a);
    drop(b);
    println!("end of main");
}
```

```output
close b.txt
end of main
```

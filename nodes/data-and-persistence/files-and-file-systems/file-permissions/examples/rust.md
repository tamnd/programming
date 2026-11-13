```rust run id=permissions-and-readonly
use std::fs;
use std::os::unix::fs::PermissionsExt;

fn mode(path: &str) -> u32 {
    fs::metadata(path).unwrap().permissions().mode() & 0o7777
}

fn main() -> std::io::Result<()> {
    fs::write("data.txt", "x")?;
    fs::set_permissions("data.txt", fs::Permissions::from_mode(0o640))?;
    println!("from_mode: {:o}", mode("data.txt"));

    // set_readonly(true) removes all of the write bits.
    let mut p = fs::metadata("data.txt")?.permissions();
    p.set_readonly(true);
    fs::set_permissions("data.txt", p)?;
    println!("readonly:  {:o}", mode("data.txt"));

    // set_readonly(false) adds the write bit for all classes.
    let mut p = fs::metadata("data.txt")?.permissions();
    p.set_readonly(false);
    fs::set_permissions("data.txt", p)?;
    println!("writable:  {:o}  (others can write now)", mode("data.txt"));

    // Use PermissionsExt to change only the bits that you want.
    let mut p = fs::metadata("data.txt")?.permissions();
    p.set_mode(p.mode() & !0o022);
    fs::set_permissions("data.txt", p)?;
    println!("fixed:     {:o}", mode("data.txt"));
    Ok(())
}
```

```output for=permissions-and-readonly
from_mode: 640
readonly:  440
writable:  662  (others can write now)
fixed:     640
```

- Verdict: stdlib. The portable `Permissions` type has only `readonly` and `set_readonly`. On Unix, `set_readonly(false)` makes the file writable for all classes, so the trait `std::os::unix::fs::PermissionsExt` with `mode`, `set_mode` and `from_mode` is the safe way to change the bits. `readonly` looks only at the write bits, and not at the user, the groups or the ACLs.

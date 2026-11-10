```rust wrong id=sh-c
use std::process::Command;

fn look_up(name: &str) {
    let out = Command::new("sh")
        .arg("-c")
        .arg(format!("echo looking up {name}"))
        .output()
        .expect("sh runs");
    print!("{}", String::from_utf8_lossy(&out.stdout));
}

fn main() {
    look_up("report.txt");
    look_up("report.txt; echo INJECTED");
}
```

```output for=sh-c
looking up report.txt
looking up report.txt
INJECTED
```

```rust run id=args
use std::process::Command;

fn look_up(name: &str) {
    let out = Command::new("echo")
        .arg("looking up")
        .arg(name)
        .output()
        .expect("echo runs");
    print!("{}", String::from_utf8_lossy(&out.stdout));
}

fn main() {
    look_up("report.txt");
    look_up("report.txt; echo INJECTED");
}
```

```output for=args
looking up report.txt
looking up report.txt; echo INJECTED
```

- Verdict: stdlib. `std::process::Command` takes the program and each argument apart. Its documentation says that arguments are passed as they are and are not evaluated by a shell.
- The weakness comes back when a program runs `sh -c` with a string from `format!`, as in the first example.
- Windows has no argument array at the system level. A program gets one command line and splits it itself, and `cmd.exe` uses its own rules for batch files. CVE-2024-24576 was a bug in the escaping of `Command` for `.bat` and `.cmd` files. The fix makes `Command` return an `InvalidInput` error when it cannot escape an argument safely.

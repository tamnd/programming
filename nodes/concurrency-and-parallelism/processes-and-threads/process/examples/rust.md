```rust run id=child
use std::process::{Command, Stdio};

const SCRIPT: &str =
    r#"echo "child sees GREETING=$GREETING"; GREETING=changed; echo "child set GREETING=$GREETING"; exit 3"#;

fn main() -> std::io::Result<()> {
    let greeting = String::from("hello");
    let child = Command::new("sh")
        .args(["-c", SCRIPT])
        .env("GREETING", &greeting)
        .stdout(Stdio::piped())
        .spawn()?;
    let pid = child.id();
    let output = child.wait_with_output()?;
    print!("{}", String::from_utf8_lossy(&output.stdout));
    println!("child exit status: {}", output.status.code().unwrap_or(-1));
    println!("child pid differs: {}", pid != std::process::id());
    println!("parent greeting: {greeting}");
    Ok(())
}
```

```output for=child
child sees GREETING=hello
child set GREETING=changed
child exit status: 3
child pid differs: true
parent greeting: hello
```

- Verdict: stdlib. `std::process::Command` is a builder; `spawn` starts the child and returns a `Child`.
- `wait_with_output` takes the `Child`, reads the piped output and waits for the exit. `status.code()` is `None` when a signal stopped the child.
- `env` adds one variable to the environment that the child inherits. It does not change the environment of the parent.
- A `Child` that is dropped is not waited for. Call `wait` or `wait_with_output` so that no zombie stays behind.

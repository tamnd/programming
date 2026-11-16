```rust run id=codes-rust
#[derive(Clone, Copy, PartialEq)]
enum Code {
    Ok = 0,
    Empty = 1,
    NotANumber = 2,
    OutOfRange = 3,
}

const NAMES: [&str; 4] = ["ok", "empty", "not a number", "out of range"];

// Returns a code. On success, writes the port.
fn parse_port(text: &str, port: &mut u16) -> Code {
    if text.is_empty() {
        return Code::Empty;
    }
    if !text.bytes().all(|b| b.is_ascii_digit()) {
        return Code::NotANumber;
    }
    match text.parse::<u16>() {
        Ok(p) => {
            *port = p;
            Code::Ok
        }
        Err(_) => Code::OutOfRange,
    }
}

fn main() {
    for text in ["8080", "", "x", "99999"] {
        let mut port = 0;
        let code = parse_port(text, &mut port);
        if code != Code::Ok {
            println!("'{text}' -> error code {} ({})", code as i32, NAMES[code as usize]);
        } else {
            println!("'{text}' -> port {port}");
        }
    }
}
```

```output for=codes-rust
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: idiom. Rust returns errors in a `Result`, so a function that returns a code is unusual. An `std::io::Error` keeps the operating system code: `raw_os_error` "Returns the OS error that this error represents (if any)".

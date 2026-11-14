```rust run id=buf-read-lines
use std::io::{BufRead, Cursor};

fn main() {
    let text = "one\ntwo\r\nthree\rfour";

    // BufRead::lines removes an LF or a CRLF. A CR alone does not end a line.
    let lines: Vec<String> = Cursor::new(text).lines().map(|l| l.unwrap()).collect();
    println!("{:?}", lines);

    // split('\n') keeps the CR of a CRLF.
    println!("{:?}", text.split('\n').collect::<Vec<_>>());

    // read_line keeps the line ending, so a program can see which one a line had.
    let mut reader = Cursor::new(text);
    let mut line = String::new();
    while reader.read_line(&mut line).unwrap() > 0 {
        let ending = if line.ends_with("\r\n") { "CRLF" } else if line.ends_with('\n') { "LF" } else { "none" };
        println!("{:?} ends with {}", line, ending);
        line.clear();
    }
}
```

```output for=buf-read-lines
["one", "two", "three\rfour"]
["one", "two\r", "three\rfour"]
"one\n" ends with LF
"two\r\n" ends with CRLF
"three\rfour" ends with none
```

- Verdict: stdlib. `BufRead::lines` removes an LF or a CRLF, and a CR alone does not end a line. `read_line` keeps the ending, so the program can see which ending each line had.

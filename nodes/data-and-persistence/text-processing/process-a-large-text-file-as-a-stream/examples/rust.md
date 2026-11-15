```rust run id=rust-stream
use std::fs::{self, File};
use std::io::{self, BufRead, BufReader, BufWriter, Write};

fn stream(path: &std::path::Path) -> io::Result<(u64, u64, Option<u64>)> {
    let mut reader = BufReader::new(File::open(path)?);
    let mut line = String::new();
    let (mut count, mut total, mut largest) = (0, 0, None);
    loop {
        line.clear(); // read_line appends to the buffer
        if reader.read_line(&mut line)? == 0 {
            break;
        }
        let n: u64 = line.trim_end().parse().map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;
        count += 1;
        total += n;
        largest = largest.max(Some(n));
    }
    Ok((count, total, largest))
}

fn main() -> io::Result<()> {
    let dir = std::env::temp_dir().join(format!("stream-{}", std::process::id()));
    fs::create_dir(&dir)?;
    let path = dir.join("numbers.txt");
    let mut w = BufWriter::new(File::create(&path)?);
    for i in 0..1_000_000 {
        writeln!(w, "{}", i % 1000)?;
    }
    w.flush()?;
    drop(w);

    let (count, total, largest) = stream(&path)?;
    println!("lines: {count} sum: {total} max: {largest:?}");
    fs::remove_dir_all(&dir)
}
```

```output
lines: 1000000 sum: 499500000 max: Some(999)
```

`BufReader` reads the file in blocks, and `read_line` adds the next line to one `String`. The loop clears the string before each read, so it holds one line at a time. `read_line` returns 0 at the end of the file.

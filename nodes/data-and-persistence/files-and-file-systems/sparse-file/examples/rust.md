```rust run id=set-len-and-blocks
use std::fs::{self, File, OpenOptions};
use std::io::{Read, Seek, SeekFrom, Write};
use std::os::unix::fs::MetadataExt;

fn main() -> std::io::Result<()> {
    let dir = std::env::temp_dir().join(format!("sparse-{}", std::process::id()));
    fs::create_dir_all(&dir)?;
    let path = dir.join("sparse.bin");

    let mut f = OpenOptions::new().write(true).create(true).truncate(true).open(&path)?;
    f.set_len(128 << 20)?; // a larger size with no write: the file is one hole
    f.seek(SeekFrom::Start(64 << 20))?;
    f.write_all(b"mid")?; // data in the middle of the hole
    drop(f);

    let meta = fs::metadata(&path)?;
    println!("len:             {}", meta.len());
    // blocks() counts 512-byte units, on Unix only.
    println!("uses less space: {}", meta.blocks() * 512 < meta.len());

    let mut f = File::open(&path)?;
    let mut head = [1u8; 4];
    f.read_exact(&mut head)?;
    println!("hole reads as:   {:?}", head);
    let mut mid = [0u8; 3];
    f.seek(SeekFrom::Start(64 << 20))?;
    f.read_exact(&mut mid)?;
    println!("at 64 MiB:       {:?}", std::str::from_utf8(&mid).unwrap());

    fs::remove_dir_all(&dir)
}
```

```output for=set-len-and-blocks
len:             134217728
uses less space: true
hole reads as:   [0, 0, 0, 0]
at 64 MiB:       "mid"
```

- Verdict: stdlib. `File::set_len` makes a file longer without a write, and a seek past the data and a write make a hole. `MetadataExt::blocks` gives the allocated space, but only on Unix. The standard library has no call for `SEEK_DATA` or `SEEK_HOLE`.

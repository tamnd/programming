```rust run id=hard-link-count
use std::fs;
use std::io::ErrorKind;
use std::os::unix::fs::{symlink, MetadataExt};

fn main() -> std::io::Result<()> {
    fs::write("data.txt", "alpha\n")?;
    fs::hard_link("data.txt", "hard.txt")?;
    symlink("data.txt", "soft.txt")?;

    let data = fs::metadata("data.txt")?;
    let hard = fs::metadata("hard.txt")?;
    println!("same inode:  {}", data.ino() == hard.ino());
    println!("nlink:       {}", data.nlink());
    println!("is_symlink:  {}", fs::symlink_metadata("soft.txt")?.file_type().is_symlink());
    println!("read_link:   {}", fs::read_link("soft.txt")?.display());

    fs::remove_file("data.txt")?;
    println!("nlink now:   {}", fs::metadata("hard.txt")?.nlink());
    let gone = fs::metadata("soft.txt").map_err(|e| e.kind());
    println!("soft.txt:    {:?}", gone.err() == Some(ErrorKind::NotFound));
    Ok(())
}
```

```output for=hard-link-count
same inode:  true
nlink:       2
is_symlink:  true
read_link:   data.txt
nlink now:   1
soft.txt:    true
```

- Verdict: stdlib. `fs::hard_link`, `fs::read_link` and `fs::symlink_metadata` are portable. The call that makes a symbolic link is per system: `std::os::unix::fs::symlink` on Unix, and `symlink_file` or `symlink_dir` in `std::os::windows::fs`. The inode and the link count come from the Unix trait `MetadataExt`.
